struct PixelInputType
{
    float4 myPosition : SV_POSITION;
    float4 myWorldPosition : POSITION;
    float4 myColor : COLOR;
    float4 myNormal : NORMAL;
    float4 myTangent : TANGENT;
    float4 myBiNormal : BINORMAL;
    float2 myUV : TEXCOORD;
};


struct TerrainPixelInputType
{
    float4 myPosition : SV_POSITION;
    float4 myWorldPosition : POSITION;
    float4 myColor : COLOR;
    float4 myNormal : NORMAL;
    float4 myTangent : TANGENT;
    float4 myBiNormal : BINORMAL;
    float2 myUV : TEXCOORD;
    float myClip : SV_ClipDistance;
};

struct VertexInputType
{
    float3 myPosition : POSITION;
    float4 myColor : COLOR;
    float3 myNormal : NORMAL;
    float3 myTangent : TANGENT;
    float3 myBiNormal : BINORMAL;
    float2 myUV : TEXCOORD;
};


struct PixelOutput
{
    float4 myColor : SV_TARGET;
};


cbuffer StaticBuffer : register(b0)
{
    float2 myResolution;
    float2 myPadding0;
}


cbuffer FrameBuffer : register(b1)
{
    float4x4 myClipSpaceMatrix;
    float myTimeDelta;
    float myTotalTime;
    float2 myPadding1;
    float4 myCameraPosition;
    float4 myLightDirection;
    float4 myDirLightColor;
    float4 myAmbientColor;
}

cbuffer ObjectBuffer : register(b2)
{
    float4x4 myObjectMatrix;
    float4 myMaterialColor;
    float4 mySize;
}







float3 GetNormal(float4 aNormalTexture)
{
    float3 normal = aNormalTexture.agg;
    normal = 2.0f * normal - 1.0f;
    normal.z = sqrt(1 - saturate((normal.x * normal.x) - (normal.y * normal.y)));
    return normalize(normal);
}


int GetNumMips(TextureCube cubeTex)
{
    int iWidth = 0;
    int iheight = 0;
    int numMips = 0;
    cubeTex.GetDimensions(0, iWidth, iheight, numMips);
    return numMips;
}


// Origin: https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
float3 s_curve(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}


float3 tonemap_s_gamut3_cine(float3 c)
{
    // based on Sony's s gamut3 cine
    float3x3 fromSrgb = float3x3(
        +0.6456794776, +0.2591145470, +0.0952059754,
        +0.0875299915, +0.7596995626, +0.1527704459,
        +0.0369574199, +0.1292809048, +0.8337616753);

    float3x3 toSrgb = float3x3(
        +1.6269474099, -0.5401385388, -0.0868088707,
        -0.1785155272, +1.4179409274, -0.2394254004,
        +0.0444361150, -0.1959199662, +1.2403560812);

    return mul(toSrgb, s_curve(mul(fromSrgb, c)));
}

