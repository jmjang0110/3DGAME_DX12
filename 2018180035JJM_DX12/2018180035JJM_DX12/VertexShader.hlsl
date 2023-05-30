
//���� ���̴��� �����Ѵ�.
float4 VS_Main(uint nVertexID : SV_VertexID) : SV_POSITION
{
    float4 output = (float4) 0;
    //������Ƽ��(�ﰢ��)�� �����ϴ� ������ �ε���(SV_VertexID)�� ���� ������ ��ȯ�Ѵ�. 
    //������ ��ġ ��ǥ�� ��ȯ�� �� ��ǥ(SV_POSITION)�̴�. ��, ������ǥ���� ��ǥ�̴�. 

    if (nVertexID == 0)
        output = float4(-1.0f, +1.0f, 0.0f, 1.0f);
    else if (nVertexID == 1)
        output = float4(+1.0f, +1.0f, 0.0f, 1.0f);
    else if (nVertexID == 2)
        output = float4(+1.0f, -1.0f, 0.0f, 1.0f);
    else if (nVertexID == 3)
        output = float4(-1.0f, +1.0f, 0.0f, 1.0f);
    else if (nVertexID == 4)
        output = float4(+1.0f, -1.0f, 0.0f, 1.0f);
    else if (nVertexID == 5)
        output = float4(-1.0f, -1.0f, 0.0f, 1.0f);
    return (output);
    
}