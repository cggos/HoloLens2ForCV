//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"
#include "ShaderStructures.h"
#include "researchmode\ResearchModeApi.h"
#include <Texture2D.h>

#include <fstream>

using namespace winrt::Windows::Storage;

namespace BasicHologram
{
    // This sample renderer instantiates a basic rendering pipeline.
    class GyroRenderer
    {
    public:
        GyroRenderer(std::shared_ptr<DX::DeviceResources> const& deviceResources, IResearchModeSensor *pAccelSensor, HANDLE hasData, ResearchModeSensorConsent *pCamAccessConsent)
        {
            m_pGyroSensor = pAccelSensor;
            m_pGyroSensor->AddRef();
            m_pAccelUpdateThread = new std::thread(GyroUpdateThread, this, hasData, pCamAccessConsent);

            StorageFolder localFolder = ApplicationData::Current().LocalFolder();
            OutputDebugString(L"localFolder.Path().data(): ");
            OutputDebugString(localFolder.Path().data());

            wchar_t fileName[MAX_PATH] = {};
            swprintf_s(fileName, L"%s\\imu_gyr.txt", localFolder.Path().data());

            of_gyr = std::ofstream(fileName);
            of_gyr << "# VinylHupTicks, SocTicks, GyroValues_x, GyroValues_y, GyroValues_z, temperature" << std::endl;
        }
        virtual ~GyroRenderer()
        {
            m_fExit = true;
            m_pAccelUpdateThread->join();

            if (m_pGyroSensor)
            {
                m_pGyroSensor->CloseStream();
                m_pGyroSensor->Release();
            }

            if (of_gyr.is_open()) of_gyr.close();
        }
        void Update(DX::StepTimer const& timer);
        void UpdateSample();

        // Repositions the sample hologram.
        void PositionHologram(winrt::Windows::UI::Input::Spatial::SpatialPointerPose const& pointerPose);

        // Property accessors.
		void SetSensorFrame(IResearchModeSensorFrame* pSensorFrame);

        void GetGyroSample(DirectX::XMFLOAT3 *pGyroSample);

    private:
        static void GyroUpdateThread(GyroRenderer* pSpinningCube, HANDLE hasData, ResearchModeSensorConsent *pCamAccessConsent);
        void GyroUpdateLoop();

        // If the current D3D Device supports VPRT, we can avoid using a geometry
        // shader just to set the render target array index.
        bool                                            m_usingVprtShaders = false;

		IResearchModeSensor *m_pGyroSensor = nullptr;
		IResearchModeSensorFrame* m_pSensorFrame;
        DirectX::XMFLOAT3 m_gyroSample;
        std::mutex m_sampleMutex;

        std::thread *m_pAccelUpdateThread;
        bool m_fExit = { false };

        std::ofstream of_gyr;
    };

}
