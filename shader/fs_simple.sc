$input v_worldPos

#include <bgfx_shader.sh>

#define MAX_LIGHTS 16

uniform vec4 u_color;

uniform vec4 u_lightPositions[MAX_LIGHTS];
uniform vec4 u_lightColors[MAX_LIGHTS];
uniform vec4 u_lightParams[MAX_LIGHTS];

uniform vec4 u_lightCount;
uniform vec4 u_ambientColor;

void main()
{
    vec3 lighting = u_ambientColor.rgb * u_ambientColor.a;

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        if (i >= int(u_lightCount.x))
            break;

        vec2 lightPos = u_lightPositions[i].xy;

        float radius = u_lightParams[i].x;
        float falloff = u_lightParams[i].y;

        float dist = distance(v_worldPos.xy, lightPos);

        float attenuation = clamp(
            1.0 - dist / max(radius, 0.0001),
            0.0,
            1.0
        );

        attenuation = pow(attenuation, falloff);

        vec3 lightColor = u_lightColors[i].rgb;
        float intensity = u_lightColors[i].a;

        lighting += lightColor * intensity * attenuation;
    }

    vec3 finalColor = u_color.rgb * lighting;

    gl_FragColor = vec4(finalColor, u_color.a);
}