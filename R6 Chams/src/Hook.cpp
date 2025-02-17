#include "minhook/include/MinHook.h" 
#include "FindPattern/FindPattern.h" 
#include "Models.hpp" 

namespace Hook
{
    constexpr void LiberarComSeguranca(IUnknown* dxObjeto)
    {
        if (dxObjeto)
        {
            dxObjeto->Release();
            dxObjeto = nullptr;
        }
    }

    constexpr void LiberarComSegurancaSRVs(ID3D11ShaderResourceView* SRVs[])
    {
        for (int i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++)
        {
            if (SRVs[i] == nullptr)
            {
                break;
            }
            LiberarComSeguranca(SRVs[i]);
        }
    }

    constexpr int ContarSRVs(ID3D11ShaderResourceView* SRVs[])
    {
        auto i = 0;
        for (; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++)
        {
            if (!SRVs[i])
            {
                break;
            }
        }
        return i;
    }

    DWORD WINAPI Iniciar()
    {
        auto statusMinGancho = MH_Initialize();
        if (statusMinGancho != MH_OK)
        {
            DEBUG_PRINT("Erro: Falha ao inicializar o MinHook! Status: %d\n", statusMinGancho);
            return false;
        }

        const auto identificadorModulo = reinterpret_cast<uintptr_t>(GetModuleHandleW(L"d3d11.dll"));
        if (!identificadorModulo)
        {
            DEBUG_PRINT("Erro: Falha ao obter o identificador do módulo d3d11.dll. Ultimo erro: %lu\n", GetLastError());
            return false;
        }
        DEBUG_PRINT("Identificador do módulo d3d11.dll: %p\n", reinterpret_cast<void*>(identificadorModulo));

        const auto inicioBusca = identificadorModulo + 0x100000;
        const auto tamanhoBusca = identificadorModulo + 0x200000 - inicioBusca;
        auto enderecoFuncao = reinterpret_cast<LPVOID>(IgroWidgets::FindPattern(inicioBusca, tamanhoBusca,
            reinterpret_cast<const uint8_t*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x00\x41\x8B\xF0\x48\x8B\xDA\x48\x8D\xB9\x00\x00\x00\xFF\x48\x8B\xCF\xE8\x00\x00\x00\x00\x84\xC0\x74\x00\x48\x85\xDB\x74\x00\x8B\x8B\x00\x00\x00\x00\x8B\x93"),
            "xxxxxxxxxxxxxx?xxxxxxxxx???xxxxx????xxx?xxxx?xx????xx"));
        if (!enderecoFuncao)
        {
            DEBUG_PRINT("Não encontramos DrawIndexedInstancedIndirect no Windows 11, tentando no Windows 10...\n");
            enderecoFuncao = reinterpret_cast<LPVOID>(IgroWidgets::FindPattern(inicioBusca, tamanhoBusca,
                reinterpret_cast<const uint8_t*>("\x48\x83\xEC\x00\x4C\x8B\xD1\x48\x85\xD2\x74\x00\x8B\x82"),
                "xxx?xxxxxxx?xx"));
        }

        DrawIndexedInstancedIndirectAddress = enderecoFuncao;
        if (!DrawIndexedInstancedIndirectAddress)
        {
            DEBUG_PRINT("Erro: Não encontramos a função DrawIndexedInstancedIndirect!\n");
            return false;
        }
        DEBUG_PRINT("Função DrawIndexedInstancedIndirect encontrada em: %p\n", DrawIndexedInstancedIndirectAddress);

        statusMinGancho = MH_CreateHook(DrawIndexedInstancedIndirectAddress, DesvioDrawIndexedInstancedIndirect, reinterpret_cast<LPVOID*>(&OriginalDrawIndexedInstancedIndirect));
        if (statusMinGancho != MH_OK)
        {
            DEBUG_PRINT("Erro: Falha ao criar o gancho para DrawIndexedInstancedIndirect! Status: %d\n", statusMinGancho);
            return false;
        }

        statusMinGancho = MH_EnableHook(DrawIndexedInstancedIndirectAddress);
        if (statusMinGancho != MH_OK)
        {
            DEBUG_PRINT("Erro: Falha ao ativar o gancho para DrawIndexedInstancedIndirect! Status: %d\n", statusMinGancho);
            return false;
        }

        DEBUG_PRINT("Gancho para DrawIndexedInstancedIndirect ativado\n");
        return true;
    }
}
