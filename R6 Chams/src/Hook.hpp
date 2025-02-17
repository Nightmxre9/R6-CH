#pragma once

#include <d3d11.h>
#include "CorRGBA.hpp"

// Definição de um ponteiro para a função DrawIndexedInstancedIndirect do Direct3D 11
typedef void(__stdcall* D3D11DesenharIndexadoInstanciadoIndiretamente) (ID3D11DeviceContext* pContexto, ID3D11Buffer* pBufferDeArgumentos, UINT DeslocamentoByteAlinhado);

namespace Gancho
{
    inline D3D11DesenharIndexadoInstanciadoIndiretamente DesenhoIndexadoInstanciadoOriginal = nullptr;
    inline LPVOID EnderecoDesenhoIndexadoInstanciado = nullptr;

    inline ID3D11DepthStencilState* EstadoStencilParedeHack = nullptr;

    inline ID3D11ShaderResourceView* texturaVermelha = nullptr;
    inline ID3D11ShaderResourceView* texturaVerde = nullptr;

    DWORD WINAPI Iniciar();
    void Liberar();

    void CriarTexturaColorida(ID3D11Device* dispositivo, const CorRGBA& cor, ID3D11ShaderResourceView** visualizadorDeRecursos);
    void CriarTexturasColoridas(ID3D11Device* dispositivo);

    void CriarEstadosStencilProfundidade(ID3D11Device* dispositivo);

    bool DesenharChams(ID3D11DeviceContext* pContexto, ID3D11Buffer* pBufferDeArgumentos, UINT DeslocamentoByteAlinhado, ID3D11ShaderResourceView* Texturas[],
        ID3D11ShaderResourceView* texturaQuandoInvisivel, ID3D11ShaderResourceView* texturaQuandoVisivel);

    void __stdcall DesvioDesenharIndexadoInstanciadoIndiretamente(ID3D11DeviceContext* pContexto, ID3D11Buffer* pBufferDeArgumentos, UINT DeslocamentoByteAlinhado);
}

