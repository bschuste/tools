/************************************************************************/
/* Date         : 28/03/95                                   			*/
/* Auteur       : Brigitte SCHUSTER                             		*/
/* Projet       : SPOT2                                         		*/
/* Version      : 1.0                                          			*/
/* Utilisation  : acc -o matrice matrice.c                          	*/
/*

 tmcc -o viSmatrice.out -B -el -host tmsim viSmatrice.c -IC:\NEXPERIA\TCS22DVP\include
 tmsim viSmatrice.out 

 *//* Editer  		: textedit -tab_width 4                          		*/
/************************************************************************/

#include <stdio.h>
#include <math.h>

void main(argc,argv)
int     argc;
char    *argv[];
{
	float	Er,Eg,Eb;	/* signaux primaires precorriges en gamma */
	float	Ey,Ecr,Ecb;	/* signaux matrices CCIR601 */
	float	Eu,Ev;	    /* differences de couleur PAL */
	int		Y,CR,CB;	/* signaux CCIR601 numerises */	
	int		R,G,B;		/* signaux RGB numerises precorriges en gamma*/	

/* Matrice M */
	double	M[3][3];
/*  [ Ey  ]	  	[  0.299	 0.587		 0.114 ] [ Er ]	*/
/*	[     ]		[ 						  	   ] [    ]	*/
/*  [ Ecr ]  =  [  0.500	-0.419		-0.081 ] [ Eg ]	*/
/*	[     ]		[ 							   ] [    ]	*/
/*  [ Ecb ] 	[ -0.169	-0.331		 0.500 ] [ Eb ]	*/


/*  [ Ey  ]	  	[  0.299	 0.587		 0.114 ] [ Er ]	*/
/*	[     ]		[ 						  	   ] [    ]	*/
/*  [ Eu  ]  =  [ -0.147	-0.290		 0.437 ] [ Eg ]	*/
/*	[     ]		[ 							   ] [    ]	*/
/*  [ Ev  ] 	[  0.615	-0.515		-0.100 ] [ Eb ]	*/

	int		j,u ;

    if (argc < 2)
    {
		printf(" -> Matricage analogique :\n");
		printf(" 	Pour le matricage digital : matrice -d\n");

		do
		{
/* Entree des valeurs Er, Eg, Eb */
		printf(" -> Entrer des valeurs entre 0 et 1 :\n");
		printf("    Er = ");
		scanf("%f",&Er);
		printf("    Eg = ");
		scanf("%f",&Eg);
		printf("    Eb = ");
		scanf("%f",&Eb);


/* Calcul des sorties -- CCIR601 */
			printf("\n -> ycrcb analogique CCIR601\n");
			Ey =  0.299*Er + 0.587*Eg +  0.114*Eb;
			printf("    Ey  = % 1.3f  \n",Ey);
			Ecr = 0.500*Er - 0.419*Eg -  0.081*Eb;
			printf("    Ecr = % 1.3f  \n",Ecr);
			Ecb = -0.169*Er - 0.331*Eg +  0.500*Eb;
			printf("    Ecb = % 1.3f  \n",Ecb);
			printf(" -> YCRCB (numerique)\n");
			Y = (int)(Ey*256);
			if (Y>235) Y=235;
			if (Y<16)  Y=16;
			CR = (int)((Ecr+0.5)*256);
			if (CR>240) CR=240;
			if (CR<16)  CR=16;
			CB = (int)((Ecb+0.5)*256);
			if (CB>240) CB=240;
			if (CB<16)  CB=16;
			printf("    Y  = %03d\t Y  = 0x%02x\n",Y,Y);
			printf("    CR = %03d\t CR = 0x%02x \n",CR,CR);
			printf("    CB = %03d\t CB = 0x%02x \n",CB,CB);

		/* Calcul des sorties -- PAL-L */
			printf(" -> yuv PAL-L\n");
			Ey =  0.299*Er + 0.587*Eg +  0.114*Eb;
			printf("    Ey = % 1.3f  \n",Ey);
			Eu = -0.147*Er - 0.290*Eg +  0.437*Eb;
			printf("    Eu = % 1.3f  \n",Eu);
			Ev =  0.615*Er - 0.515*Eg -  0.100*Eb;
			printf("    Ev = % 1.3f  \n",Ev);
			printf("\n\n");
		} while (getchar()!='q');

    } else
	{
		printf("\t -> Matricage digital avec clipping entre 16 et 235/240 :\n");
		do
		{
		/* Entree des valeurs R, G, B */
			printf(" -> Entrer des valeurs entre 0 et 255 :\n");
			printf("    R = ");
			scanf("%d",&R);
			printf("    G = ");
			scanf("%d",&G);
			printf("    B = ");
			scanf("%d",&B);


		/* Calcul des sorties -- CCIR601 */
			printf("\n");
			Y  = (int)(0.299*R + 0.587*G +  0.114*B);
			CR = (int)(0.500*R - 0.419*G -  0.081*B);
			CB = (int)(-0.169*R - 0.331*G +  0.500*B);
			if (Y>235) Y=235;
			if (Y<16)  Y=16;
			CR = CR+128;
			if (CR>240) CR=240;
			if (CR<16)  CR=16;
			CB = CB+128;
			if (CB>240) CB=240;
			if (CB<16)  CB=16;
			printf("    -> Y  = %03d\t Y  = 0x%02x\n",Y,Y);
			printf("    -> CR = %03d\t CR = 0x%02x  == V !\n",CR,CR);
			printf("    -> CB = %03d\t CB = 0x%02x  == U !\n\n",CB,CB);
		} while (getchar()!='q');
	}

}		
