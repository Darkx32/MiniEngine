$input v_worldPos

#include <bgfx_shader.sh>

#define MAX_LIGHTS 16
#define MAX_SHADOW_SEGMENTS 32

uniform vec4 u_color;

uniform vec4 u_lightPositions[MAX_LIGHTS];
uniform vec4 u_lightColors[MAX_LIGHTS];
uniform vec4 u_lightParams[MAX_LIGHTS];

uniform vec4 u_lightCount;
uniform vec4 u_ambientColor;

uniform vec4 u_shadowSegments[MAX_SHADOW_SEGMENTS];
uniform vec4 u_shadowCount;
uniform vec4 u_shadowIgnore;

float cross2D(vec2 a, vec2 b)
{
    return a.x * b.y - a.y * b.x;
}

bool intersectSegments(
    vec2 a,
    vec2 b,
    vec2 c,
    vec2 d
)
{
    vec2 r = b - a;
    vec2 s = d - c;

    float denominator = cross2D(r, s);

    if (abs(denominator) < 0.00001)
        return false;

    vec2 diff = c - a;

    float t = cross2D(diff, s) / denominator;
    float u = cross2D(diff, r) / denominator;

    const float EPSILON = 0.0001;

    return t > EPSILON
        && t < 1.0 - EPSILON
        && u >= 0.0
        && u <= 1.0;
}

float calculateVisibility(
    vec2 lightPos,
    vec2 fragmentPos
)
{
    int ignoreStart = int(u_shadowIgnore.x);
    int ignoreCount = int(u_shadowIgnore.y);

    for (int i = 0; i < MAX_SHADOW_SEGMENTS; ++i)
    {
        if (i >= int(u_shadowCount.x))
            break;

        int start = int(u_shadowIgnore.x);
        int count = int(u_shadowIgnore.y);

        if (i >= start && i < start + count)
            continue;

        vec4 segment = u_shadowSegments[i];

        if (intersectSegments(
            lightPos,
            fragmentPos,
            segment.xy,
            segment.zw
        ))
        {
            return 0.0;
        }
    }

    return 1.0;
}

void main()
{
    vec3 lighting =
        u_ambientColor.rgb * u_ambientColor.a;

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        if (i >= int(u_lightCount.x))
            break;

        vec2 lightPos = u_lightPositions[i].xy;
        vec2 lightDir = u_lightPositions[i].zw;

        float radius = u_lightParams[i].x;
        float falloff = u_lightParams[i].y;

        vec2 toFragment = v_worldPos.xy - lightPos;

        float dist = length(toFragment);

        float attenuation = clamp(
            1.0 - dist / max(radius, 0.0001),
            0.0,
            1.0
        );

        attenuation = pow(attenuation, falloff);

        if (attenuation <= 0.0)
            continue;

        if (dot(lightDir, lightDir) > 0.5)
        {
            vec2 fragmentDir =
                toFragment / max(dist, 0.0001);

            float theta = dot(
                fragmentDir,
                lightDir
            );

            float innerCutoff = u_lightParams[i].z;
            float outerCutoff = u_lightParams[i].w;

            float epsilon = max(
                innerCutoff - outerCutoff,
                0.0001
            );

            float spotFactor = clamp(
                (theta - outerCutoff) / epsilon,
                0.0,
                1.0
            );

            spotFactor =
                spotFactor * spotFactor
                * (3.0 - 2.0 * spotFactor);

            attenuation *= spotFactor;
        }

        if (attenuation <= 0.0)
            continue;

        float visibility = calculateVisibility(
            lightPos,
            v_worldPos.xy
        );

        if (visibility < 0.5)
            continue;

        vec3 lightColor = u_lightColors[i].rgb;
        float intensity = u_lightColors[i].a;

        lighting +=
            lightColor * intensity * attenuation;
    }

    vec3 finalColor = u_color.rgb * lighting;

    gl_FragColor = vec4(
        finalColor,
        u_color.a
    );
}