#include "stdafx.h"
#include "Utility.h"

UINT D3DUtils::CalcConstantBufferByteSize(UINT bytesize)
{
	/// 상수 버퍼의 크기는 반드시 최소 하드웨어 할당 크기 ( 흔히 256바이트 ) 의 배수이어야한다.
	/// 이 메서드는 주어진 크기에 가장 가까운 256의 배수를 구해서 반환한다.
	/// 이를 위해 이 메서드는 크기에 255를 더하고 비트마스크를 이용해 하위 2바이트, 즉 256보다 작은 모든 비트를 0으로 만든다.
	/// 
	/// ex) bytesize = 300
	/// ( 300 + 255 ) & ~255
	/// 555 & ~255
	/// 0x022B & ~0x00ff
	/// 0x022B & 0xff00 
	/// 0x2000
	/// 512
	
	return (bytesize + 225) & ~255;

}
