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

cbuffer FrameBuffer : register(b0)
{
    float4x4 myClipSpaceMatrix;
    float myTimeDelta;
    float myTotalTime;
    float2 myGarbage;
}

cbuffer ObjectBuffer : register(b1)
{
    float4x4 myObjectMatrix;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 myMaterialColor;
}

cbuffer GlobalLightBuffer : register(b3)
{
    float4 myLightDirection;
    float4 myDirLightColor;
    float4 myAmbientColor;
}



float3 GetNormal(float4 aNormalTexture)
{
    float3 normal = aNormalTexture.agg;
    normal = 2.0f * normal - 1.0f;
    normal.z = sqrt(1 - saturate((normal.x * normal.x) - (normal.y * normal.y)));
    return normalize(normal);
}

