// BaseColor.fx

// 1. 자동완성기능이 없다.
// 2. 오탈자 검사가 없다.
// 3. hlsl - high level shader language 이녀석도 확장자로 쓸 수 있음
// .hlsl(일반적인 확장자, 내가 몰쓸지 모르면 이녀석으로) 
// .fx(효과) 
// .ps(픽셀 쉐이더) 
// .vs(버텍스 쉐이더)

// 4. 변수가 달라짐  -> Vector2, Matrix 존재 하지 않음
//					-> float2, float4x4 (Vector3 float3임)

/*
	쉐이더 내에서 사용하는 함수들이 몇몇가지 존재하기는 함
*/

// 전역변수
// 외부에서 들어오는 값 (내가 지정한 값), 쉐이더 동작할 때 필요한 값 세팅

// 외부에서 넣을때는 스트링으로 검색해서 넣게 되어있음
// 파라미터는 전부 string으로 검사
// ex) ("a", 10.0f) 이런식으로
//		("b", D3DXVECTOR4(0,0,0,0))
// ("a", 5.0f") // 중복되면 마지막 녀석만 들어감 
// string -> 

// 오타 조심
float4x4 matWorld;
float4x4 matView;
float4x4 matProjection;

// vs 버텍스 쉐이더
// 버텍스 쉐이더의 들어가는 값
// 버텍스 숫자만큼 버텍스 쉐이더가 돌아감
// 버텍스 INPUT은 외부에서 값 들어감
struct VS_INPUT
{
	// FVF 랑 같음 (컴퓨터에 POSITION로 쓸 거라 알려주는거)
    float4 position : POSITION; // POSITION은 시멘틱 
    float4 color : COLOR0;
};

// ps 픽셀 쉐이더
// 픽셀한테 들어감
// 버텍스 쉐이더에서 계산된 녀석이 픽셀 쉐이더한테 들어갈 구조체임
struct PS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR0;
};

// 함수 만드신거 따로 함수 존재하는게 아니라
// 버텍스 수만큼 실행됨
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    output.position = mul(input.position, matWorld);
    output.position = mul(output.position, matView);
    output.position = mul(output.position, matProjection);

    output.color = input.color;
    return output; // 따로 설정안해줘도 자동으로 픽셀 쉐이더로 들어감
}

// 버텍스가 화면상에 어디위치하는지 보고 그 안에 픽셀값을 설정해줌
// 안에들어간 픽셀 수만큼 실행됨 
// ex)100 x 100 일때 10004 번 실행됨
// 사각형 vertex 4번이라 VS 4번 실행 1000 x 1000 백만번 실행
float4 PS(PS_INPUT input) : COLOR0 // 반환되는 float4 어떤 녀석으로 쓸건지
{
    return input.color;
}

// 테크닉
// 쉐이더 묶어서 상황에 따라 나눠서 쓰고 싶을때
technique MyShader
{
	// pass 여러개 들어가도 됨 단 이름과 상관없이 pass 위에서 녀석부터 돌아감
    pass P0
    {
		// vs_3_0 , ps_3_0 compile 버전 지금까지 나온 버전 5.1까지인가 나옴
		// 버전 올라갈 수록 쉐이더 하나로 데이터 집어 넣어서 계산할 수 있는 능력이 다름
		// 3_0 한번에 집어넣고 계산할때 만개 정도
		// 5_0 십만개 이상
		// 버전 낮으면 쉐이더 여러번 사용해야됨
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS();
    }
}

// vector값은 무조건 vector4 로 넣어주어야 함 vector2 쓰더라도
// b 녀석 x,y 만 쓰더라도 float 2,3,4 는 vector4로 넣어야함
// ex) ("b", D3DXVECTOR4(x,y,0,0)
//float a;
//float2 b;
//float4x4 c;
// 이렇게 쓰면 클래스에서 검사할 때 a or b or c로 검사해야됨