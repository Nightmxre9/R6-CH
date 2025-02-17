#pragma once
struct CorRGBA
{
    unsigned char Vermelho;
    unsigned char Verde;
    unsigned char Azul;
    unsigned char Alfa;

    CorRGBA(unsigned char vermelho = 0, unsigned char verde = 0, unsigned char azul = 0, unsigned char alfa = 255) :
        Vermelho(vermelho), Verde(verde), Azul(azul), Alfa(alfa) {}
};
