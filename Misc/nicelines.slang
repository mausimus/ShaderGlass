#version 450

// === PARAMETERS ===
#pragma parameter COMPARE_MODE "[v26] Half Screen COMPARE" 0.0 0.0 1.0 1.0
#pragma parameter SCANLINE_MODE "180p / 224p / 240p / 270p / 360p / MANUAL" 2.0 0.0 5.0 1.0
#pragma parameter MANUAL_LINES "MANUAL Scanline Count" 360.0 120.0 480.0 5.0
#pragma parameter AUTO_LINES "ROUND Scanline Count to Multiple of Resolution" 0.0 0.0 1.0 1.0
#pragma parameter BLUR_OFFSET "BEAM FOCUS" 0.04 -0.04 0.12 0.01
#pragma parameter H_BLUR "HORIZONTAL BLUR (worn out tube look)" 1.0 0.0 8.0 0.1

// === SUBPIXEL NOISE PARAMETERS ===
#pragma parameter SUBPIXEL_NOISE "SHADOW MASK Off/Swap/Triad" 2.0 0.0 2.0 1.0
#pragma parameter SHADOW_STRENGTH "Shadow Mask STRENGTH (1-3)" 2.0 1.0 3.0 1.0
#pragma parameter SHIMMER_NOISE "Shimmering NOISE Effect (Requires Shadow Mask Effect ON)" 0.0 0.0 1.0 1.0

// === TRIAD PARAMETERS ===
#pragma parameter PHOSPHOR_WIDTH "TRIAD Phosphor Width (px)" 1.0 1.0 20.0 1.0
#pragma parameter TRIAD_HEIGHT "TRIAD Height (px)" 1.0 1.0 20.0 1.0
#pragma parameter SPILLOVER_MODE "TRIAD Spillover (Linear/Curved)" 0.0 0.0 1.0 1.0
#pragma parameter CURVE_PRIMARY "TRIAD Curve PRIMARY (0=linear, -=concave, +=convex)" 0.0 -3.0 3.0 0.1
#pragma parameter CURVE_SECONDARY "TRIAD Curve SECONDARY (0=linear, -=concave, +=convex)" 0.0 -3.0 3.0 0.1
#pragma parameter CURVE_TERTIARY "TRIAD Curve TERTIARY (0=linear, -=concave, +=convex)" 0.0 -3.0 3.0 0.1

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float COMPARE_MODE;
    float SCANLINE_MODE;
    float MANUAL_LINES;
    float AUTO_LINES;
    float BLUR_OFFSET;
    float H_BLUR;
    float SUBPIXEL_NOISE;
    float SHADOW_STRENGTH;
    float SHIMMER_NOISE;
    float PHOSPHOR_WIDTH;
    float TRIAD_HEIGHT;
    float SPILLOVER_MODE;
    float CURVE_PRIMARY;
    float CURVE_SECONDARY;
    float CURVE_TERTIARY;
} global;

// === VERTEX STAGE ===
#pragma stage vertex
layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 0) out vec2 vTexCoord;

void main() {
    gl_Position = global.MVP * Position;
    vTexCoord = TexCoord;
}

// === FRAGMENT STAGE ===
#pragma stage fragment
layout(location = 0) in vec2 vTexCoord;
layout(location = 0) out vec4 FragColor;
layout(set = 0, binding = 2) uniform sampler2D Source;

// sRGB conversion
float SrgbToLinear(float c) {
    return c <= 0.04045 ? c / 12.92 : pow((c + 0.055) / 1.055, 2.4);
}

float LinearToSrgb(float c) {
    return c <= 0.0031308 ? c * 12.92 : 1.055 * pow(c, 1.0/2.4) - 0.055;
}

vec3 SrgbToLinear3(vec3 c) {
    return vec3(SrgbToLinear(c.r), SrgbToLinear(c.g), SrgbToLinear(c.b));
}

vec3 LinearToSrgb3(vec3 c) {
    return vec3(LinearToSrgb(c.r), LinearToSrgb(c.g), LinearToSrgb(c.b));
}

// Calculate scanline width for a given color
float calcScanlineWidth(vec3 color, float baseWidth, float minWidth, float maxWidth) {
    float maxColorChannel = max(max(color.r, color.g), color.b);
    float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
    
    // Dark path: interpolate minWidth → baseWidth based on luma
    float t = clamp(luma / baseWidth, 0.0, 1.0);
    float darkWidth = mix(minWidth, baseWidth, t);
    
    // Bright path: width follows max channel
    float brightWidth = clamp(maxColorChannel, baseWidth, maxWidth);
    
    // Smooth blend between dark and bright paths around baseWidth threshold
    float blendZone = 0.1;
    float blend = smoothstep(baseWidth - blendZone * baseWidth, baseWidth + blendZone * baseWidth, maxColorChannel);
    
    return mix(darkWidth, brightWidth, blend);
}

// Sample source with optional blur
vec3 sampleWithBlur(vec2 uv, float effectiveScale) {
    vec3 color;
    if(effectiveScale < 0.999) {
        float blurRadius = (1.0 / effectiveScale - 1.0) * 0.5;
        vec2 texelSize = 1.0 / global.SourceSize.xy;
        vec2 radius = texelSize * blurRadius;
        
        // 9-tap box filter for area averaging
        vec3 sum = vec3(0.0);
        sum += texture(Source, uv + vec2(-radius.x, -radius.y)).rgb;
        sum += texture(Source, uv + vec2(      0.0, -radius.y)).rgb;
        sum += texture(Source, uv + vec2( radius.x, -radius.y)).rgb;
        sum += texture(Source, uv + vec2(-radius.x,       0.0)).rgb;
        sum += texture(Source, uv).rgb;
        sum += texture(Source, uv + vec2( radius.x,       0.0)).rgb;
        sum += texture(Source, uv + vec2(-radius.x,  radius.y)).rgb;
        sum += texture(Source, uv + vec2(      0.0,  radius.y)).rgb;
        sum += texture(Source, uv + vec2( radius.x,  radius.y)).rgb;
        color = sum / 9.0;
    } else {
        color = texture(Source, uv).rgb;
    }
    return color;
}

// Sample source with optional H_BLUR applied (5-tap tent filter)
vec3 sampleWithHBlur(vec2 uv, float effectiveScale, float hBlur) {
    if(hBlur > 0.001) {
        float pixelSize = 1.0 / global.OutputSize.x;
        float blurRadius = hBlur * pixelSize;
        
        vec3 cC = SrgbToLinear3(sampleWithBlur(uv, effectiveScale));
        vec3 cL1 = SrgbToLinear3(sampleWithBlur(vec2(uv.x - blurRadius * 0.5, uv.y), effectiveScale));
        vec3 cL2 = SrgbToLinear3(sampleWithBlur(vec2(uv.x - blurRadius, uv.y), effectiveScale));
        vec3 cR1 = SrgbToLinear3(sampleWithBlur(vec2(uv.x + blurRadius * 0.5, uv.y), effectiveScale));
        vec3 cR2 = SrgbToLinear3(sampleWithBlur(vec2(uv.x + blurRadius, uv.y), effectiveScale));
        
        return cL2 * 0.1 + cL1 * 0.2 + cC * 0.4 + cR1 * 0.2 + cR2 * 0.1;
    } else {
        return SrgbToLinear3(sampleWithBlur(uv, effectiveScale));
    }
}

void main() {
    float baseWidth = 0.3;   // Hardcoded thickness baseline
    float minWidth = 0.5;    // Hardcoded thickness min
    float maxWidth = 1.0;    // Hardcoded thickness max
    
    // Determine target scanline count based on mode
    float targetLines;
    int mode = int(global.SCANLINE_MODE + 0.5);
    if(mode == 0) {
        targetLines = 180.0;
    } else if(mode == 1) {
        targetLines = 224.0;
    } else if(mode == 2) {
        targetLines = 240.0;
    } else if(mode == 3) {
        targetLines = 270.0;
    } else if(mode == 4) {
        targetLines = 360.0;
    } else {
        targetLines = global.MANUAL_LINES;
    }
    
    // Auto-fit: find nearest factor of output height to target
    if(global.AUTO_LINES > 0.5) {
        float outputHeight = global.OutputSize.y;
        float idealPixelsPerLine = outputHeight / targetLines;
        float pixelsPerLine = max(1.0, round(idealPixelsPerLine));
        targetLines = outputHeight / pixelsPerLine;
    }
    
    // Source blur settings
    float baselineScale = targetLines / global.SourceSize.y;
    float effectiveScale = clamp(baselineScale + global.BLUR_OFFSET, 0.02, 1.0);
    
    // Pixels per scanline (for subpixel noise phase calculation)
    float pixelsPerScanline = global.OutputSize.y / targetLines;
    
    // Edge softness calculation (hardcoded to full softening)
    float baseEdgeSoftness = 0.05;
    float curvedAmount = pow(1.0, 2.5);  // VERTICAL_AMOUNT = 1.0
    float edgeSoftness = baseEdgeSoftness + curvedAmount * 0.25;
    
    // Compare mode: right half shows original
    if(global.COMPARE_MODE > 0.5 && vTexCoord.x > 0.5) {
        vec3 color = texture(Source, vTexCoord).rgb;
        FragColor = vec4(color, 1.0);
        return;
    }
    
    // === SUBPIXEL NOISE: Calculate displaced sample position ===
    vec2 px = vTexCoord * global.OutputSize.xy;
    vec2 srcPx = px;
    
    // Mode selection: 0=off, 1=swap, 2=triad
    int shadowMode = int(global.SUBPIXEL_NOISE + 0.5);
    
    if(shadowMode == 1) {
        // Mode 1: Original horizontal swap shadow mask
        // Seed: random each frame if shimmer enabled, otherwise fixed
        float seed = (global.SHIMMER_NOISE > 0.5) ? float(global.FrameCount) : 0.0;
        
        // --- HORIZONTAL SWAP ---
        // Swap range directly from strength setting (1, 2, or 3)
        float swapRange = global.SHADOW_STRENGTH;
        if(swapRange > 0.5) {
            // Phase height is a factor of scanline pitch, rounded to integer fat-pixels (locked at 0.8)
            float phaseH = max(1.0, floor(pixelsPerScanline * 0.8 + 0.5));
            
            float groupWidth = swapRange * 2.0;
            float groupIdx = floor(px.x / groupWidth);
            float withinGroup = mod(px.x, groupWidth);
            
            float rowPhase = floor(px.y / phaseH);
            
            float swapOffset = 0.0;
            if(mod(rowPhase, 2.0) < 1.0) {
                swapOffset = groupWidth - 1.0 - 2.0 * withinGroup;
            } else {
                swapOffset = mod(withinGroup + swapRange, groupWidth) - withinGroup;
            }
            
            float chaos = sin((groupIdx + rowPhase) * 2.718 + seed) * swapRange * 0.5;
            swapOffset += chaos;
            
            srcPx.x += swapOffset;
        }
    }
    
    // Convert displaced pixel position to UV
    vec2 displacedUV = srcPx / global.OutputSize.xy;
    displacedUV = clamp(displacedUV, vec2(0.0), vec2(1.0));
    
    // Calculate scanline position for displaced UV (CENTER-ANCHORED)
    float distFromCenter = displacedUV.y - 0.5;
    float scanlineOffset = distFromCenter * targetLines;
    float scanlinePos = scanlineOffset;
    float scanlineIndex = floor(scanlinePos);
    
    // Sample at scanline center
    float sampleScanlinePos = scanlineIndex + 0.5;
    float sampleDistFromCenter = sampleScanlinePos / targetLines;
    vec2 sampleUV = vec2(displacedUV.x, clamp(0.5 + sampleDistFromCenter, 0.0, 1.0));
    
    float withinScanline = fract(scanlinePos);
    
    // === SAMPLE SOURCE WITH HORIZONTAL BLUR (applied early so scanlines and triads both see it) ===
    vec3 color = sampleWithHBlur(sampleUV, effectiveScale, global.H_BLUR);
    
    // Calculate scanline width and energy-preserved color
    float scanlineWidth = calcScanlineWidth(color, baseWidth, minWidth, maxWidth);
    vec3 outputColor = clamp(color / scanlineWidth, 0.0, 1.0);
    
    // Scanline mask with soft edges
    float scanlineMask = 1.0;
    vec3 finalColor;
    float halfGap = (1.0 - scanlineWidth) * 0.5;
    
    scanlineMask = smoothstep(halfGap - edgeSoftness, halfGap + edgeSoftness, withinScanline) *
                   smoothstep(1.0 - halfGap + edgeSoftness, 1.0 - halfGap - edgeSoftness, withinScanline);
    
    finalColor = outputColor * scanlineMask;
    
    // Compare mode divider line
    if(global.COMPARE_MODE > 0.5 && abs(vTexCoord.x - 0.5) < 0.002) {
        finalColor = vec3(0.0);
    }

    // === MODE 2: TRIAD-BASED SHADOW MASK (APPLIED LAST) ===
    // Configurable phosphor width and triad height
    if(shadowMode == 2) {
        float phosphorW = global.PHOSPHOR_WIDTH;
        float triadW = phosphorW * 3.0;  // Total triad width = 3 phosphors
        float triadH = global.TRIAD_HEIGHT;
        
        // Which triad column are we in?
        float triadIdxX = floor(px.x / triadW);
        float withinTriadX = mod(px.x, triadW);
        int phosphorIdx = int(floor(withinTriadX / phosphorW));  // 0=left, 1=middle, 2=right
        phosphorIdx = min(phosphorIdx, 2);  // Clamp to valid range
        
        // Vertical zigzag offset for alternating columns (when triad height > 1)
        // Offset = ceil(triadH / 2) for odd columns
        float zigzagOffset = 0.0;
        if(triadH > 1.0 && mod(triadIdxX, 2.0) >= 1.0) {
            zigzagOffset = ceil(triadH / 2.0);
        }
        
        // Apply zigzag to Y position before calculating triad row
        float adjustedPxY = px.y - zigzagOffset;
        float triadIdxY = floor(adjustedPxY / triadH);
        
        // === AREA SAMPLING: Integrate the ENTIRE triad footprint ===
        // Sample the FULLY PROCESSED image (blur + H_BLUR + scanlines) at each point
        // Triad bounds in output pixel space (with zigzag applied)
        float triadLeftPx = triadIdxX * triadW;
        float triadTopPx = triadIdxY * triadH + zigzagOffset;
        
        // Sample grid: one sample per output pixel in the triad
        int samplesX = max(int(triadW), 1);
        int samplesY = max(int(triadH), 1);
        
        vec3 totalEnergy = vec3(0.0);
        float sampleCount = 0.0;
        
        for(int sy = 0; sy < samplesY; sy++) {
            for(int sx = 0; sx < samplesX; sx++) {
                // Output pixel position (center of each pixel in the triad)
                float samplePxX = triadLeftPx + float(sx) + 0.5;
                float samplePxY = triadTopPx + float(sy) + 0.5;
                vec2 samplePxPos = vec2(samplePxX, samplePxY);
                
                // Convert to UV
                vec2 sampleUVPos = samplePxPos / global.OutputSize.xy;
                sampleUVPos = clamp(sampleUVPos, vec2(0.0), vec2(1.0));
                
                // === Compute full scanline processing for this sample point ===
                // Calculate scanline position (CENTER-ANCHORED)
                float sDistFromCenter = sampleUVPos.y - 0.5;
                float sScanlineOffset = sDistFromCenter * targetLines;
                float sScanlinePos = sScanlineOffset;
                float sScanlineIndex = floor(sScanlinePos);
                
                // Sample UV at scanline center (vertical snap)
                float sSampleScanlinePos = sScanlineIndex + 0.5;
                float sSampleDistFromCenter = sSampleScanlinePos / targetLines;
                vec2 sSampleUV = vec2(sampleUVPos.x, clamp(0.5 + sSampleDistFromCenter, 0.0, 1.0));
                
                float sWithinScanline = fract(sScanlinePos);
                
                // Sample source with blur + H_BLUR
                vec3 sColor = sampleWithHBlur(sSampleUV, effectiveScale, global.H_BLUR);
                
                // Calculate scanline width and energy-preserved color
                float sScanlineWidth = calcScanlineWidth(sColor, baseWidth, minWidth, maxWidth);
                vec3 sOutputColor = clamp(sColor / sScanlineWidth, 0.0, 1.0);
                
                // Scanline mask with soft edges
                float sHalfGap = (1.0 - sScanlineWidth) * 0.5;
                float sScanlineMask = smoothstep(sHalfGap - edgeSoftness, sHalfGap + edgeSoftness, sWithinScanline) *
                                      smoothstep(1.0 - sHalfGap + edgeSoftness, 1.0 - sHalfGap - edgeSoftness, sWithinScanline);
                
                // Final processed color at this sample point
                vec3 sProcessedColor = sOutputColor * sScanlineMask;
                
                totalEnergy += sProcessedColor;
                sampleCount += 1.0;
            }
        }
        
        // Average processed color in the triad area
        vec3 avgColor = totalEnergy / sampleCount;
        
        // Apply brightness boost
        avgColor *= 1.1;
        
        // Total energy to distribute across 3 phosphors (multiply by 3)
        float totalRed = avgColor.r * 3.0;
        float totalGreen = avgColor.g * 3.0;
        float totalBlue = avgColor.b * 3.0;
        
        // Spillover distribution
        float redLeft, redMiddle, redRight;
        float greenLeft, greenMiddle, greenRight;
        float blueLeft, blueMiddle, blueRight;
        
        if(global.SPILLOVER_MODE < 0.5) {
            // === LINEAR SPILLOVER (original) ===
            // Distribute RED: left first, then right, then middle
            redLeft = min(totalRed, 1.0);
            float redRemaining = max(totalRed - 1.0, 0.0);
            redRight = min(redRemaining, 1.0);
            redRemaining = max(redRemaining - 1.0, 0.0);
            redMiddle = min(redRemaining, 1.0);
            
            // Distribute GREEN: middle first, then left, then right
            greenMiddle = min(totalGreen, 1.0);
            float greenRemaining = max(totalGreen - 1.0, 0.0);
            greenLeft = min(greenRemaining, 1.0);
            greenRemaining = max(greenRemaining - 1.0, 0.0);
            greenRight = min(greenRemaining, 1.0);
            
            // Distribute BLUE: right first, then middle, then left
            blueRight = min(totalBlue, 1.0);
            float blueRemaining = max(totalBlue - 1.0, 0.0);
            blueMiddle = min(blueRemaining, 1.0);
            blueRemaining = max(blueRemaining - 1.0, 0.0);
            blueLeft = min(blueRemaining, 1.0);
        } else {
            // === CURVED SPILLOVER ===
            // Each tier has its own curve: slider 0 = linear, negative = concave, positive = convex
            // Exponent = 2^slider: -1 → 0.5 (concave), 0 → 1 (linear), +1 → 2 (convex)
            float expPrimary = pow(2.0, global.CURVE_PRIMARY);
            float expSecondary = pow(2.0, global.CURVE_SECONDARY);
            float expTertiary = pow(2.0, global.CURVE_TERTIARY);
            
            // RED: primary=left, secondary=right, tertiary=middle
            float rNorm = totalRed / 3.0;  // Normalize to 0-1 range
            redLeft = pow(clamp(rNorm, 0.0, 1.0), expPrimary);
            redRight = pow(clamp(rNorm, 0.0, 1.0), expSecondary);
            redMiddle = pow(clamp(rNorm, 0.0, 1.0), expTertiary);
            
            // GREEN: primary=middle, secondary=left, tertiary=right
            float gNorm = totalGreen / 3.0;
            greenMiddle = pow(clamp(gNorm, 0.0, 1.0), expPrimary);
            greenLeft = pow(clamp(gNorm, 0.0, 1.0), expSecondary);
            greenRight = pow(clamp(gNorm, 0.0, 1.0), expTertiary);
            
            // BLUE: primary=right, secondary=middle, tertiary=left
            float bNorm = totalBlue / 3.0;
            blueRight = pow(clamp(bNorm, 0.0, 1.0), expPrimary);
            blueMiddle = pow(clamp(bNorm, 0.0, 1.0), expSecondary);
            blueLeft = pow(clamp(bNorm, 0.0, 1.0), expTertiary);
        }
        
        // Select the color values for this phosphor
        if(phosphorIdx == 0) {
            // Left phosphor
            finalColor = vec3(redLeft, greenLeft, blueLeft);
        } else if(phosphorIdx == 1) {
            // Middle phosphor
            finalColor = vec3(redMiddle, greenMiddle, blueMiddle);
        } else {
            // Right phosphor
            finalColor = vec3(redRight, greenRight, blueRight);
        }
    } else {
        // Apply brightness boost for non-triad modes (moved here to avoid double-apply)
        finalColor *= 1.1;
    }

    FragColor = vec4(LinearToSrgb3(finalColor), 1.0);
}
