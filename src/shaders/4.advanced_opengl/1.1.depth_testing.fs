#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth) {
    float ndc = depth * 2.0f - 1.0f;
    float linearDepth = (2.0 * near * far) / (far + near - ndc * (far - near));
    return linearDepth;
}
void main() {
    // float depth = LinearizeDepth(gl_FragCoord.z) / far;
    // FragColor = vec4(vec3(depth), 1.0f);
    FragColor = texture(texture1, TexCoords);
}