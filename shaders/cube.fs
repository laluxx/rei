#version 330

// Input from vertex shader
in vec2 fragTexCoord;
in vec3 fragPosition;
in vec3 fragNormal;

// Output color
out vec4 finalColor;

// Uniforms
uniform float timeValue;

void main() {
    // Create a pulsating color effect
    float r = abs(sin(timeValue * 1.5));
    float g = abs(sin(timeValue * 2.0));
    float b = abs(sin(timeValue * 2.5));
    
    // Simple lighting based on normal
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(fragNormal, lightDir), 0.2);
    
    // Apply the color with lighting
    vec3 color = vec3(r, g, b);
    finalColor = vec4(color, 1.0);
}
