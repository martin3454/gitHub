

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

void nakonec(Node **tail, uint8_t x, uint8_t y){

	Cord pom;
	pom.x = x;
	pom.y = y;
	Node *pt = cre(pom);
	pt->next = NULL;
	pt->prev = *tail;
	(*tail)->next = pt;
	*tail = pt;
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

		Cord bod={
				50,
				35
		};

		Cord souradnice[3]={

			{11,10},
			{12,10},
			{13,10},
			//{14,10},
			//{15,10},
			//{16,10},
			//{17,10},
			//{18,10},
			//{19,10},
			//{20,10},
			//{21,10},
			//{22,10},
			//{23,10},

		};

		for(int i = 0;i < 3; ++i){

			list.head = listik(list.head, &list.tail, souradnice[i]);
		}

		for(Node *pt = list.head; pt != NULL; pt = pt->next){

			GLCD_SetPixel(pt->souradnice.x, pt->souradnice.y, 1);

		}
		GLCD_SetPixel(bod.x, bod.y, 1);



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

					//kontrola bodu
					if(ocas->prev->souradnice.x == bod.x && ocas->prev->souradnice.y == bod.y){

						GLCD_SetPixel(bod.x, bod.y, 0);
						bod.x +=5;
						GLCD_SetPixel(bod.x, bod.y, 1);
						int x = list.tail->souradnice.x - list.tail->prev->souradnice.x;
						int y = list.tail->souradnice.y - list.tail->prev->souradnice.y;
						nakonec(&list.tail, list.tail->souradnice.x + x, list.tail->souradnice.y + y);

					}
					break;
				}

				//mazani bodu
				GLCD_SetPixel(ocas->souradnice.x, ocas->souradnice.y, 0);

				ocas->souradnice = ocas->prev->souradnice;
				ocas = ocas->prev;
			}

			//vykresleni
			for(Node *pt = list.head; pt != NULL; pt = pt->next){
				GLCD_SetPixel(pt->souradnice.x, pt->souradnice.y, 1);
				cekej(100000);
			}

		}

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////






