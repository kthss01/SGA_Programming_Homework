// TextureMapping.fx

// 한번에 곱해진 녀석을 받아와도 됨
float4x4 matWorld;
float4x4 matView;
float4x4 matProjection;

// 쉐이더 자체에서 계산해도 됨
// 외부값은 사실 flaot4로 들어가게됨
float2 maxFrame;
float2 currentFrame;

struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

// 버텍스 쉐이더는 반환될때 꼭 포지션이 있어야함
struct VS_OUTPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

	// postion float4 로 넣는 이유가 4x4 matrix랑 곱해야되서
	// 다른 방법으론 3x3 matrix로 바꾸면 되긴 함 (곱할때 필요없는거 빼서)
	// 형 변환은 c랑 똑같이 (float3x3) 이런식으로 하면 됨
	// float3 -> float4 로 강제 형변환은 안되고
	// float4 temp = float4(input.position, 1.0f);

	// 계산이 지저분하게 나올 수 도 있음 그런경우 w값 초기화
    input.position.w = 1.0f;
	
    output.position = mul(input.position, matWorld);
    output.position = mul(output.position, matView);
    output.position = mul(output.position, matProjection);

	// uv 좌표는 픽셀 쉐이더에서 설정
    output.uv = input.uv;

    return output;
}

// 외부에서 텍스처 받으려면 변수 선언 필요
texture tex;

// 텍스처가 uv 좌표값 넘어갔을때 어떻게 출력할지 설정
// 텍스처 크기보다 크거나 작을때 어떻게 처리할지 설정
// 외부에서 지정한거 가져다 쓸 수 있고
// 내부에서 지정해서 쓸 수도 있음
// LINEAR 그래픽카드에 영향 덜 주는거 멀리있는거 다 뭉개뜨리는
sampler2D tex_sampler = sampler_state
{
    Texture = tex;
	// 텍스처가 원본사이즈보다 커질 때 처리
    MAGFILTER = LINEAR;
	// 텍스처가 원본사이즈보다 작아질 때 처리
    MINFILTER = LINEAR;
	// 텍스처가 원본사이즈보다 같을 때 처리
    MIPFILTER = LINEAR;
};

// 픽셀 쉐이더에서는 포지션이 없어도 됨
struct PS_INPUT
{
    float2 uv : TEXCOORD0;
};

// 반환되는 값은 무조건 color 텍스처에 대한 컬러값이 반환되야되서
float4 PS(PS_INPUT input) : COLOR0
{
    float2 temp;
    temp.x = input.uv.x / maxFrame.x + currentFrame.x / maxFrame.x;
    temp.y = input.uv.y / maxFrame.y + currentFrame.y / maxFrame.y;
	
	// 자체 내에서 컬러값 수정해도 됨 RGBA 순
    return tex2D(tex_sampler, temp) * float4(1.0f, 0, 0, 0.5f);
}

technique MyShader
{
    pass Pass_0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS();
    }
}