#ifndef WIN32_D3D11_H
#define WIN32_D3D11_H

// enum render_api {
//     D3D11,
// };

// struct render_context {
//     render_api renderAPI;

// };


global IDXGISwapChain* globalD3D11SwapChain;
global ID3D11Device* globalD3D11Device;
global ID3D11DeviceContext* globalD3D11DeviceContext;

global ID3D11RenderTargetView* globalD3D11BackBuffer;
global ID3D11DepthStencilView* globalD3D11DepthBuffer;

global ID3D11Buffer* globalD3D11VertexBuffer;
global ID3D11Buffer* globalD3D11IndexBuffer;
global ID3D11InputLayout* globalD3D11InputLayout;

global ID3D11VertexShader* globalD3D11VertexShader;
global ID3D11PixelShader* globalD3D11PixelShader;

global ID3D11Buffer* globalConstantBufferPerObjectBuffer;
global XMMATRIX globalWVP;
global XMMATRIX globalWorld;
global XMMATRIX globalCamView;
global XMMATRIX globalCamProjection;

global XMVECTOR globalCamPos;
global XMVECTOR globalCamTarget;
global XMVECTOR globalCamUp;

struct constant_buffer_per_obj {
    XMMATRIX WVP;
};

global constant_buffer_per_obj cbPerObj;

#endif //WIN32_D3D11_H