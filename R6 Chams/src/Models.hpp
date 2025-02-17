#pragma

#include <d3d11.h>

#pragma warning(disable:4267)

namespace Modelos 
{
    // Função para verificar se o formato da Shader Resource View (SRV) corresponde ao formato alvo
    constexpr bool CombinarFormatoSRV(ID3D11ShaderResourceView* srv, int formatoAlvo)
    {
        if (srv)
        {
            if (formatoAlvo > -1)
            {
                D3D11_SHADER_RESOURCE_VIEW_DESC descricao;
                srv->GetDesc(&descricao);
                return descricao.Format == formatoAlvo;
            }
            return true;
        }
        return false;
    }

    class Modelo 
    {
    public:
        int Passo; // Stride -> Passo (distância entre vértices)
        int LarguraVSC; // VscWidth -> LarguraVSC
        int LarguraPSC; // PscWidth -> LarguraPSC
        int FormatoSRV; // SrvFormat -> FormatoSRV
        int IndiceSRV; // SrvIndex -> IndiceSRV

        Modelo(int passo, int larguraVSC, int larguraPSC, int formatoSRV = -1, int indiceSRV = -1)
            : Passo(passo), LarguraVSC(larguraVSC), LarguraPSC(larguraPSC), FormatoSRV(formatoSRV), IndiceSRV(indiceSRV)
        {
        }

        bool EhModelo(UINT passo, UINT larguraVSC, UINT larguraPSC, ID3D11ShaderResourceView* SRVs[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]) const
        {
            auto resultado = (Passo < 0 || passo == static_cast<unsigned>(Passo)) &&
                             (LarguraVSC < 0 || larguraVSC == static_cast<unsigned>(LarguraVSC)) &&
                             (LarguraPSC < 0 || larguraPSC == static_cast<unsigned>(LarguraPSC));

            if (resultado)
            {
                if (SRVs && (IndiceSRV >= 0 || FormatoSRV >= 0))
                {
                    if (IndiceSRV < 0)
                    {
                        resultado = false;
                        for (auto i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++)
                        {
                            if (!SRVs[i])
                            {
                                break;
                            }

                            if (CombinarFormatoSRV(SRVs[i], FormatoSRV))
                            {
                                resultado = true;
                                break;
                            }
                        }
                    }
                    else
                    {
                        resultado = IndiceSRV < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT &&
                                    CombinarFormatoSRV(SRVs[IndiceSRV], FormatoSRV);
                    }
                }
            }

            return resultado;
        }
    };

    // Modelos específicos
    inline Modelo Pele1(8, 62, 4, 77, 2);
    inline Modelo Pele2(8, 62, 4, 83, 2);
    inline Modelo Corpo1(8, 62, 12, 83);
    inline Modelo Corpo2(8, 62, 14, 83);

    inline Modelo DroneSobreposicaoMundo(8, 49, 3);
    inline Modelo SobreposicaoMundo1(8, 49, 4);
    inline Modelo SobreposicaoMundo2(8, 49, 12);
    inline Modelo SobreposicaoMundo3(8, 49, 14);
    inline Modelo SobreposicaoMundo4(8, 49, 49);

    inline Modelo Mira(8, 49, 49, 77, 2); // Texturas da mira
}
