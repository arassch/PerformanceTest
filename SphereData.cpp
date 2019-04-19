#include "FrameBuffer.h"
#include "SphereData.h"

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <chrono>

#define INITIAL_ANGLE  1.57f
#define ANGLE_ROTATION 0.1f

CSphereData::CSphereData( const char *szFilename, CFrameBuffer* fb ) :
    fb(fb)
{
    FILE *in;
    in = fopen(szFilename,"rb");

	int num = 0;
	srand(1);
	for(;;)
	{
		SSphereElement el;

        if(fscanf(in,"%f %f %f",&el.x,&el.y,&el.z)!=3)
			break;

		el.y-=60;
		el.z-=50;

		el.x*=0.01f;
		el.y*=0.01f;
		el.z*=0.01f;

		el.r = 5.0f + 5.0f * (rand()%1024)/1024.0f;
		el.r *= 0.004f;
//		el.dwARGB = rand()&0xff;
//        el.dwARGB = (el.dwARGB<<8) | (rand()&0xff);
//        el.dwARGB = (el.dwARGB<<8) | (rand()&0xff);
        el.red = ((float) rand() / (RAND_MAX))*255.0;
        el.green = ((float) rand() / (RAND_MAX))*255.0;
        el.blue = ((float) rand() / (RAND_MAX))*255.0;

		m_SphereData.push_back(el);
		//if (num++ > 100) break;
	}

	fclose(in);

    m_wi = INITIAL_ANGLE;
    m_fAnimateAngleRatio = ANGLE_ROTATION;
    m_renderedAngle = 0;
}

CSphereData::~CSphereData()
{
}

inline bool CompareSpheresFunc( const SSphereElement &s1,const SSphereElement &s2 )
{
	return s1.screenZ > s2.screenZ;
}

void CSphereData::Render( )
{
    printf("Started rendering\n");
    bool fullTurn = false;
    while(!fullTurn)
    {
        std::chrono::steady_clock::time_point beginTime = std::chrono::steady_clock::now();

        fb->m_renderingBuffer1 = !fb->m_renderingBuffer1;
        fb->Clear();

        std::vector<SSphereElement>::iterator it, end=m_SphereData.end();

        float s=sin(m_wi);
        float c=cos(m_wi);

        for(it=m_SphereData.begin();it!=end;++it)
        {
            SSphereElement &ref = *it;
            ref.screenZ = ref.x*c+ref.z*s;
        }

        std::sort( m_SphereData.begin(),m_SphereData.end(),CompareSpheresFunc );

        for(it=m_SphereData.begin();it!=end;++it)
        {
            const SSphereElement &ref = *it;

            float fX = ref.x*s-ref.z*c;
            float fY = ref.y;
            //float fZ = ref.x*c+ref.z*s;
            float fZ = ref.screenZ;

            fZ += 1.5f;

            if(fZ<0.001f)
                continue;

            float fScreenX = fX/fZ;
            float fScreenY = fY/fZ;
            float fScreenZ = fZ;

//            fb->RenderSphere(fScreenX,fScreenY,fScreenZ,ref.r/fZ,ref.dwARGB);
            fb->RenderSphere(fScreenX,fScreenY,fScreenZ,ref.r/fZ,ref.red, ref.green, ref.blue);
        }

        std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();

        unsigned int elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime).count();
        m_times.push_back(elapsedTime);

        m_wi += m_fAnimateAngleRatio;
        m_renderedAngle += m_fAnimateAngleRatio;
        if (m_wi >= 2*M_PI)
        {
            m_wi = 0;
        }

        if(m_renderedAngle >= 2*M_PI)
        {
            fullTurn = true;
        }

    }

}
