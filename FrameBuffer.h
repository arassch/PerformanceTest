#pragma once

#include <vector>

class CFrameBuffer
{
public:
	CFrameBuffer( const int iWidth, const int iHeight );
	~CFrameBuffer();

	void Clear();

	//! \param fScreenX [-1..1]
	//! \param fScreenY [-1..1]
	//! \param fScreenZ ]0..1[
	//! \param fScreenRadius >0 (-1..1 = 2 means full screen)
//	void RenderSphere( float fScreenX, float fScreenY, float fScreenZ, float fScreenRadius, unsigned int ARGB );
    void RenderSphere( float fScreenX, float fScreenY, float fScreenZ, float fScreenRadius, float red, float green, float blue );

    const unsigned char* GetFrameBuffer() const;
	int GetWidth() const { return m_iWidth; }
	int GetHeight() const { return m_iHeight; }

    bool m_renderingBuffer1;

private:

    unsigned char * m_FramebufferArray1;
    unsigned char * m_FramebufferArray2;
//    std::vector<unsigned char> m_ZBuffer;
	int m_iWidth,m_iHeight;
};
