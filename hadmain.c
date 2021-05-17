/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <header.h>
#include <stdio.h>
#include <stdlib.h>

#define M_SW1  (1<<4)
#define M_SW2  (1<<5)
#define M_SW3  (1<<16)
#define M_SW4  (1<<17)


typedef struct{
	uint8_t x;
	uint8_t y;
}Cord;


typedef struct Node{
	Cord souradnice;
	struct Node *next;
	struct Node *prev;
}Node;


typedef struct{
	Node *head;
	Node *tail;
}Seznam;



Node* cre(Cord cor){
	Node *pt=(Node*)malloc(sizeof(Node));
	pt->next = NULL;
	pt->prev = NULL;
	pt->souradnice.x = cor.x;
	pt->souradnice.y = cor.y;

	return pt;
}


Node* listik(Node *pt, Node **tail, Cord cor){

	if(pt == NULL){
		pt = cre(cor);
		*tail = pt;

	}else {
		Node *pom = cre(cor);
		pom->next = pt;
		pt->prev = pom;
		pt = pom;
	}

	return pt;
}

int main(void)
{

		GLCD_Init();

		PORTA->PCR[4] = 0x100;
		PORTA->PCR[5] = 0x100;
		PORTA->PCR[16] = 0x100;
		PORTA->PCR[17] = 0x100;
		PTA->PDDR &= ~(1<<4);  		//tlacitko 1-4 jako vstup
		PTA->PDDR &= ~(1<<5);
		PTA->PDDR &= ~(1<<16);
		PTA->PDDR &= ~(1<<17);


		Seznam list ={
				NULL,
				NULL
		};

		Cord souradnice[13]={

			{11,10},
			{12,10},
			{13,10},
			{14,10},
			{15,10},
			{16,10},
			{17,10},
			{18,10},
			{19,10},
			{20,10},
			{21,10},
			{22,10},
			{23,10},

		};

		for(int i = 0;i < 13; ++i){

			list.head = listik(list.head, &list.tail, souradnice[i]);
		}

		for(Node *pt = list.head; pt != NULL; pt = pt->next){

			GLCD_SetPixel(pt->souradnice.x, pt->souradnice.y, 1);

		}


		Node *hlava = list.head;
		Node *ocas = list.tail;
		char smer = 'r';
		int krokX = 1;
		int krokY = 0;

		while(1){

			//posun
			for(Node *ocas = list.tail;;){

				//dole
				if(((PTA->PDIR & M_SW1) == 0) && (smer == 'r' || smer == 'l')){
					krokX = 0;
					krokY = 1;
					smer = 'd';
				}

				//up
				if(((PTA->PDIR & M_SW2) == 0) && (smer == 'r' || smer == 'l')){
					krokX = 0;
					krokY = -1;
					smer = 'u';
				}

				//left
				if(((PTA->PDIR & M_SW3) == 0) && (smer == 'd' || smer == 'u') ){
					krokX = -1;
					krokY = 0;
					smer = 'l';
				}

				//right
				if(((PTA->PDIR & M_SW4) == 0) && (smer == 'd' || smer == 'u')){
					krokX = 1;
					krokY = 0;
					smer = 'r';
				}

				if(ocas->prev->prev == NULL){
					ocas->souradnice = ocas->prev->souradnice;
					ocas->prev->souradnice.x += krokX;
					ocas->prev->souradnice.y += krokY;
					break;
				}

				GLCD_SetPixel(ocas->souradnice.x, ocas->souradnice.y, 0);
				ocas->souradnice = ocas->prev->souradnice;
				ocas = ocas->prev;
			}

			//vykresleni
			for(Node *pt = list.head; pt != NULL; pt = pt->next){
				GLCD_SetPixel(pt->souradnice.x, pt->souradnice.y, 1);
				cekej(10000);
			}
		}




		/*int ocasX,ocasY, hlavaX, hlavaY, prevHlavaX, prevHlavaY, krok, rozdil, start;
		ocasX=11;
		ocasY=10;
		hlavaX=19;
		hlavaY=10;
		prevHlavaX=hlavaX;
		prevHlavaY=hlavaY;
		start=1;

		char tlacitko;






		while(1){

				if(start){
					GLCD_SetPixel(ocasX++, ocasY,0);

					GLCD_SetPixel(++hlavaX, hlavaY,1);
					cekej(1000000);
				}

				if(hlavaX == 127 || hlavaY == 63 || hlavaX == 0 || hlavaY == 0) break;

				if((PTA->PDIR & M_SW1) == 0 ) tlacitko='d';

				if((PTA->PDIR & M_SW2) == 0 ) tlacitko='u';

				if((PTA->PDIR & M_SW3) == 0 ) tlacitko='l';

				if((PTA->PDIR & M_SW4) == 0 ) tlacitko='r';

				switch(tlacitko){


				case 'd':{


					start=0;
					rozdil=hlavaX-ocasX;
					krok=1;
					if(rozdil < 0) {
						rozdil *= -1;
						krok = -1;
					}
					for(int mazani=0; mazani < rozdil; mazani+=1){
							GLCD_SetPixel(ocasX, ocasY,0);
							ocasX+=krok;
							GLCD_SetPixel(hlavaX, ++hlavaY,1);
							cekej(1000000);
					}
					ocasX=hlavaX;

					while(1){
						GLCD_SetPixel(ocasX, ocasY++,0);
						GLCD_SetPixel(hlavaX, ++hlavaY,1);
						if(hlavaY >= 63) break;
						cekej(1000000);

						if((PTA->PDIR & M_SW1) == 0 ) continue;

						if((PTA->PDIR & M_SW2) == 0 ) continue;

						if((PTA->PDIR & M_SW3) == 0 ){
							tlacitko='l';
							break;
						}
						if((PTA->PDIR & M_SW4) == 0 ){
							tlacitko='r';
							break;
						}
					}
				}
				break;

				case 'u':{

					start=0;
					rozdil=hlavaX-ocasX;
					krok=1;
					if(rozdil < 0) {
						rozdil *= -1;
						krok = -1;
					}
					for(int mazani=0; mazani < rozdil; mazani+=1){
						GLCD_SetPixel(ocasX, ocasY,0);
						ocasX+=krok;
						GLCD_SetPixel(hlavaX, --hlavaY,1);
						cekej(1000000);
					}

					ocasX=hlavaX;

					while(1){
						GLCD_SetPixel(ocasX, ocasY--,0);
						GLCD_SetPixel(hlavaX, --hlavaY,1);
						if(hlavaY <= 0) break;
						cekej(1000000);

						if((PTA->PDIR & M_SW1) == 0 ) continue;

						if((PTA->PDIR & M_SW2) == 0 ) continue;

						if((PTA->PDIR & M_SW3) == 0 ){
							tlacitko='l';
							break;
						}
						if((PTA->PDIR & M_SW4) == 0 ){
							tlacitko='r';
							break;
						}
					}
				}
				break;

				case 'l':{
					start=0;
					rozdil = hlavaY - ocasY;
					krok = 1;
					if(rozdil < 0){
						rozdil *= -1;
						krok = -1;
					}
					for(int mazani=0; mazani < rozdil; mazani++){
						GLCD_SetPixel(ocasX, ocasY,0);
						ocasY+=krok;
						GLCD_SetPixel(--hlavaX, hlavaY,1);
						cekej(1000000);
					}

					ocasY=hlavaY;

					while(1){

						GLCD_SetPixel(ocasX--, ocasY,0);
						GLCD_SetPixel(--hlavaX, hlavaY,1);
						if(hlavaX <= 0) break;
						cekej(1000000);

						if((PTA->PDIR & M_SW1) == 0 ){
							tlacitko='d';
							break;
						}
						if((PTA->PDIR & M_SW2) == 0 ){
							tlacitko='u';
							break;
						}
						if((PTA->PDIR & M_SW3) == 0 ) continue;
						if((PTA->PDIR & M_SW4) == 0 ) continue;
					}

				}
				break;

				case 'r':{
					start=0;

					rozdil=hlavaY - ocasY;
					krok=1;
					if(rozdil < 0){
						rozdil *= -1;
						krok = -1;
					}

					for(int mazani=0; mazani < rozdil; mazani+=1){
						GLCD_SetPixel(ocasX, ocasY,0);
						ocasY+=krok;
						GLCD_SetPixel(++hlavaX, hlavaY,1);
						cekej(1000000);
					}

					ocasY=hlavaY;

					while(1){
						GLCD_SetPixel(ocasX++, ocasY,0);
						GLCD_SetPixel(++hlavaX, hlavaY,1);
						if(hlavaX >= 127) break;
						cekej(1000000);

						if((PTA->PDIR & M_SW1) == 0 ){
							tlacitko='d';
							break;
						}
						if((PTA->PDIR & M_SW2) == 0 ){
							tlacitko='u';
							break;
						}
						if((PTA->PDIR & M_SW3) == 0 ) continue;
						if((PTA->PDIR & M_SW4) == 0 ) continue;
					}
				}
				break;

				}
			}*/



















    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////






