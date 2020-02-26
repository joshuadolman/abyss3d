#ifndef WIN32_D3D11_CPP
#define WIN32_D3D11_CPP

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include <xnamath.h>

#include "win32_d3d11.h"

internal void
win32_D3D11_Init(HWND window) {
	
    // Swap Chain creation
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
	swapChainDesc.BufferCount       = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Width  = WINDOW_WIDTH;
	swapChainDesc.BufferDesc.Height = WINDOW_HEIGHT;
	swapChainDesc.OutputWindow      = window;
	swapChainDesc.SampleDesc.Count  = 1;
	swapChainDesc.Windowed          = TRUE;
	swapChainDesc.Flags             = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, &swapChainDesc, &globalD3D11SwapChain, &globalD3D11Device, 0, &globalD3D11DeviceContext);
	

    // Back Buffer creation
	ID3D11Texture2D* backBufferPointer;
	globalD3D11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPointer);
	globalD3D11Device->CreateRenderTargetView(backBufferPointer, 0, &globalD3D11BackBuffer);
	backBufferPointer->Release();

    
    // Depth Buffer creation
    D3D11_TEXTURE2D_DESC depthDesc = {0};
    depthDesc.Width              = WINDOW_WIDTH;
    depthDesc.Height             = WINDOW_HEIGHT;
    depthDesc.MipLevels          = 1;
    depthDesc.ArraySize          = 1;
    depthDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count   = 1;
    // depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage              = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
    depthDesc.CPUAccessFlags     = 0;
    depthDesc.MiscFlags          = 0;

    ID3D11Texture2D* depthBufferPointer;
    globalD3D11Device->CreateTexture2D(&depthDesc, 0, &depthBufferPointer);
    globalD3D11Device->CreateDepthStencilView(depthBufferPointer, 0, &globalD3D11DepthBuffer);
    depthBufferPointer->Release();


    // Setting render targets
	globalD3D11DeviceContext->OMSetRenderTargets(1, &globalD3D11BackBuffer, 0);
    globalD3D11DeviceContext->OMSetRenderTargets(1, &globalD3D11BackBuffer, globalD3D11DepthBuffer);
	

    // Set viewport
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width    = WINDOW_WIDTH;
	viewport.Height   = WINDOW_HEIGHT;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
	
	globalD3D11DeviceContext->RSSetViewports(1, &viewport);

}


internal void
TEMP_win32_D3D11_UpdateCamera(f32 pos) {
    globalCamPos      = XMVectorSet( 0.0f,  0.0f,  pos,     0.0f);
    globalCamTarget   = XMVectorSet( 0.0f,  0.0f,  0.0f,    0.0f);
    globalCamUp       = XMVectorSet( 0.0f,  1.0f,  0.0f,    0.0f);

    globalCamView = XMMatrixLookAtLH( globalCamPos, globalCamTarget, globalCamUp);
    globalCamProjection = XMMatrixPerspectiveFovLH( 0.4f*PI32, (f32)WINDOW_WIDTH/(f32)WINDOW_HEIGHT, 1.0f, 1000.0f);

    globalWorld = XMMatrixIdentity();

    globalWVP = globalWorld * globalCamView * globalCamProjection;
}

internal void
win32_D3D11_InitGraphics() {
	
    ID3D10Blob* VS;
	ID3D10Blob* PS;

    HRESULT createShaderErrorResult = {};
	createShaderErrorResult = D3DX11CompileFromFile("shaders/shadertest.shdr", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	if(createShaderErrorResult!=0) {
        win32_OutputDebug("Uh oh - Vertex Shader Error\n");
    }
    createShaderErrorResult = D3DX11CompileFromFile("shaders/shadertest.shdr", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);
	if(createShaderErrorResult!=0) {
        win32_OutputDebug("Uh oh - Pixel Shader Error\n");
    }

    globalD3D11Device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &globalD3D11VertexShader);
    globalD3D11Device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &globalD3D11PixelShader);

    globalD3D11DeviceContext->VSSetShader(globalD3D11VertexShader, 0, 0);
	globalD3D11DeviceContext->PSSetShader(globalD3D11PixelShader, 0, 0);

    vertex myVerts[7] = {0};
	myVerts[0].position = {-0.5f, -0.5f,  0.2f};
	myVerts[0].color = {1.0f, 0.0f, 0.0f, 1.0f};
	
	myVerts[1].position = {-0.5f,  0.5f,  0.2f};
	myVerts[1].color = {0.0f, 1.0f, 0.0f, 1.0f};
	
	myVerts[2].position = { 0.5f,  0.5f,  0.2f};
	myVerts[2].color = {0.0f, 0.0f, 1.0f, 1.0f};

    myVerts[3].position = { 0.5f, -0.5f, 0.2f};
	myVerts[3].color = {1.0f, 1.0f, 1.0f, 1.0f};
    

    myVerts[4].position = {0.7f, -0.8f, 0.8f};
	myVerts[4].color = {0.0f, 1.0f, 0.0f, 1.0f};

    myVerts[5].position = {-0.2f, -0.7f, 0.8f};
	myVerts[5].color = {1.0f, 0.0f, 0.0f, 1.0f};

    myVerts[6].position = {0.0f, 0.9f, 0.8f};
	myVerts[6].color = {1.0f, 0.0f, 0.0f, 1.0f};
	
    DWORD indices[9] = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
    };

    D3D11_BUFFER_DESC bufferDescInds = {0};
	bufferDescInds.Usage = D3D11_USAGE_DEFAULT;
	bufferDescInds.ByteWidth = sizeof(DWORD)*ArrayCount(indices);
	bufferDescInds.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// bufferDescInds.CPUAccessFlags = 0;
    // bufferDescInds.MiscFlags = 0;
    
    D3D11_BUFFER_DESC bufferDescVerts = {0};
	bufferDescVerts.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescVerts.ByteWidth = sizeof(vertex) * ArrayCount(myVerts);
	bufferDescVerts.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescVerts.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    // bufferDescVerts.MiscFlags = 0;

	
		
	// D3D11_MAPPED_SUBRESOURCE vertexBufferData = {0};
	// globalD3D11DeviceContext->Map(globalD3D11VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexBufferData);
	// memcpy(vertexBufferData.pData, myVerts, sizeof(myVerts));
	// globalD3D11DeviceContext->Unmap(globalD3D11VertexBuffer, 0);
    
    D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
    vertexBufferData.pSysMem = myVerts;

    D3D11_SUBRESOURCE_DATA indexBufferData = {0};
	// globalD3D11DeviceContext->Map(globalD3D11IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &indexBufferData);
	// memcpy(vertexBufferData.pData, indices, sizeof(indices));
	// globalD3D11DeviceContext->Unmap(globalD3D11IndexBuffer, 0);
    indexBufferData.pSysMem = indices;

    globalD3D11Device->CreateBuffer(&bufferDescVerts, &vertexBufferData, &globalD3D11VertexBuffer);
	globalD3D11Device->CreateBuffer(&bufferDescInds, &indexBufferData, &globalD3D11IndexBuffer);
		
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	
	
	globalD3D11Device->CreateInputLayout(inputElementDesc, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &globalD3D11InputLayout);
    globalD3D11DeviceContext->IASetIndexBuffer(globalD3D11IndexBuffer, DXGI_FORMAT_R32_UINT,0);
	globalD3D11DeviceContext->IASetInputLayout(globalD3D11InputLayout);

    VS->Release();
    PS->Release();

    D3D11_BUFFER_DESC constantBufferDesc = {0};
    constantBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
    constantBufferDesc.ByteWidth      = sizeof(cbPerObj);
    constantBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    // constantBufferDesc.CPUAccessFlags = 0;
    // constantBufferDesc.MiscFlags      = 0;

    globalD3D11Device->CreateBuffer(&constantBufferDesc, 0, &globalConstantBufferPerObjectBuffer);

    TEMP_win32_D3D11_UpdateCamera(-10.0f);

}

internal void
win32_D3D11_Clean(void) {
	globalD3D11SwapChain->SetFullscreenState(FALSE, 0);
	globalD3D11SwapChain->Release();
	globalD3D11Device->Release();
	globalD3D11DeviceContext->Release();

    globalD3D11BackBuffer->Release();
	globalD3D11DepthBuffer->Release();
	
    globalD3D11IndexBuffer->Release();
    globalD3D11VertexBuffer->Release();
    globalD3D11InputLayout->Release();

	globalD3D11VertexShader->Release();
	globalD3D11PixelShader->Release();

    globalConstantBufferPerObjectBuffer->Release();
}

internal void
win32_D3D11_RenderFrame(void) {
	globalD3D11DeviceContext->ClearRenderTargetView(globalD3D11BackBuffer, D3DXCOLOR(0.5f, 0.0f, 0.6f, 1.0f));
	globalD3D11DeviceContext->ClearDepthStencilView(globalD3D11DepthBuffer, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
    
    cbPerObj.WVP = XMMatrixTranspose(globalWVP);
    globalD3D11DeviceContext->UpdateSubresource( globalConstantBufferPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    globalD3D11DeviceContext->VSSetConstantBuffers(0, 1, &globalConstantBufferPerObjectBuffer);
    
    vertex myVerts[7] = {0};
	// select which vertex buffer to display
	u32 stride = sizeof(vertex);
	u32 offset = 0;
	globalD3D11DeviceContext->IASetVertexBuffers(0, 1, &globalD3D11VertexBuffer, &stride, &offset);
	
	// select which primtive type we are using
	globalD3D11DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// draw the vertex buffer to the back buffer
	// globalD3D11DeviceContext->Draw(ArrayCount(myVerts), 0);
    globalD3D11DeviceContext->DrawIndexed(9,0,0);
	
	globalD3D11SwapChain->Present(0, 0);
}

#endif //WIN32_D3D11_CPP