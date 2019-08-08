uniform sampler2D texture;
uniform float intensity;

void main() {
    vec4 texel = texture2D(texture, gl_TexCoord[0].xy);

    gl_FragColor = gl_Color * vec4(texel.rgb * intensity, texel.a);
}
