#include "HEADERTBFO.h"

FILE *F1;

ListToken LT;

Stack ST;

int counter = 0;

int checkRule(char *str)
{
    if(strcmp(str, "begin")==0)
        return 1;
    else if(strcmp(str, "end")==0)
        return 1;
    else if(strcmp(str, "do")==0)
        return 1;
    else if(strcmp(str, "while")==0)
        return 1;
    else if(strcmp(str, "if")==0)
        return 1;
    else if(strcmp(str, "then")==0)
        return 1;
    else if(strcmp(str, "else")==0)
        return 1;
    else if(strcmp(str, "for")==0)
        return 1;
    else if(strcmp(str, "input")==0)
        return 1;
    else if(strcmp(str, "output")==0)
        return 1;
	else if(strcmp(str, "to")==0)
		return 1;
    else
        return 0;
}

void token(char *str, int baris)
{
    char ptr[100];
    char temp2[10];
    char *temp1;
    int i;
	int a;

	temp1 = strtok(strcat(str, " ")," 	\n");
    if(temp1 == NULL)
	{
		InsertLast(&LT, Alokasi("NIL", baris));
	}
	else
	{
		strcpy(ptr, temp1);


		do
		{

			if(checkRule(ptr))
			{
				InsertLast(&LT, Alokasi(ptr, baris));
				//printf("%s\n", ptr);
			}
			else
			{
				i = 0;
				while(ptr[i] != '\0')
				{
				temp2[0] = ptr[i];
				temp2[1] = '\0';

				InsertLast(&LT, Alokasi(temp2, baris));

				//printf("%s\n", temp2);

				i++;
				}
			}

			temp1 = strtok(NULL," 	\n");
			if (temp1 != NULL) {
				strcpy(ptr, temp1);
			}

		}while(temp1!=NULL);
	}
}

void BacaToken() {
	char str[100];
	int baris;

	F1 = fopen ("InputSource", "r");
	if (F1) {
		baris = 0;
		while (fgets(str, sizeof(str), F1) != Nil) {
			baris++;
			if (str[0] == '\n') {
				InsertLast(&LT, Alokasi("NIL", baris));
			}
			else {
				token(str, baris);
			}
		}
	} else {
		printf("gagal buka\n");
	}
	fclose(F1);
}

void PrintListToken (ListToken L) {
	Address PP;
	PP = FirstToken(L);

	while (PP != Nil) {
		printf("nama token: %s\n", InfoToken(PP).namaToken);
		printf("baris token: %d\n", InfoToken(PP).barisToken);
		PP = NextToken(PP);
	}
}

Address GetPrev(ListToken L) {
	Address P;
	P = FirstToken(L);

	while (P != Nil) {
		if (NextToken(P) == Nil) {
			break;
		}
		P = NextToken(P);
	}
	return P;
}

Address GetBefore(ListToken L, Address P) {
	Address P1;
	P1 = FirstToken(L);
	while (P1 != Nil) {
		if (NextToken(P1) == P) {
			return P1;
			break;
		}
		P1 = NextToken(P1);
	}
}

//(a+(b+()))

int EXP(Address *P, char s[]) {
	Address PP, PPtemp;
	AddressStack PS;

	PP = GetBefore(LT, *P);
	CreateStack (&ST);
	/** Mengecek validitas tanda () **/
	while (InfoToken(PP).barisToken == InfoToken(*P).barisToken && strcmp(InfoToken(PP).namaToken, "NIL") != 0 && strcmp(InfoToken(PP).namaToken, "begin") != 0 && strcmp(InfoToken(PP).namaToken, "end") != 0 && strcmp(InfoToken(PP).namaToken, "if") != 0 && strcmp(InfoToken(PP).namaToken, "else") != 0 && strcmp(InfoToken(PP).namaToken, "then") != 0 && strcmp(InfoToken(PP).namaToken, "do") != 0 && strcmp(InfoToken(PP).namaToken, "while") != 0 && strcmp(InfoToken(PP).namaToken, "for") != 0 && strcmp(InfoToken(PP).namaToken, "{") != 0 && strcmp(InfoToken(PP).namaToken, "}") != 0) {
		if (strcmp(InfoToken(PP).namaToken, "(")==0) {
			Push(&ST, "(");
		}
		else if (strcmp(InfoToken(PP).namaToken, ")")==0) {
			Pop (&ST);
		}
		PP = NextToken(PP);
	}

	int error = 0;
	int errorTO = 0, foundTO = 0;
	
	if (isStackEmpty(ST)) {
		/** Mengecek ada tidaknya TO sebagai identifier untuk FOR **/
		if (strcmp(s, "for") == 0) {
			PP = GetBefore(LT, *P);
			while (InfoToken(PP).barisToken == InfoToken(*P).barisToken && strcmp(InfoToken(PP).namaToken, "NIL") != 0 && strcmp(InfoToken(PP).namaToken, "begin") != 0 && strcmp(InfoToken(PP).namaToken, "end") != 0 && strcmp(InfoToken(PP).namaToken, "if") != 0 && strcmp(InfoToken(PP).namaToken, "else") != 0 && strcmp(InfoToken(PP).namaToken, "then") != 0 && strcmp(InfoToken(PP).namaToken, "do") != 0 && strcmp(InfoToken(PP).namaToken, "while") != 0 && strcmp(InfoToken(PP).namaToken, "for") != 0 && strcmp(InfoToken(PP).namaToken, "{") != 0 && strcmp(InfoToken(PP).namaToken, "}") != 0) {
				if (strcmp(InfoToken(PP).namaToken, "to")==0) {
					foundTO++;
				}
				PP = NextToken(PP);
			}
			if (foundTO > 1) {
				printf("INVALID identifier TO in EXP after FOR from row %d, more than 1\n", InfoToken(GetBefore(LT, *P)).barisToken);
				errorTO = 1;
			}
			else if (foundTO == 0) {
				printf("CANNOT FIND identifier TO in EXP after FOR from row %d\n", InfoToken(GetBefore(LT, *P)).barisToken);
				errorTO = 1;
			}
		}
		
		/** Mengecek validitas operator **/
		if (errorTO == 0) {
		PP = GetBefore(LT, *P);
		while (InfoToken(PP).barisToken == InfoToken(*P).barisToken && strcmp(InfoToken(PP).namaToken, "NIL") != 0 && strcmp(InfoToken(PP).namaToken, "begin") != 0 && strcmp(InfoToken(PP).namaToken, "end") != 0 && strcmp(InfoToken(PP).namaToken, "if") != 0 && strcmp(InfoToken(PP).namaToken, "else") != 0 && strcmp(InfoToken(PP).namaToken, "then") != 0 && strcmp(InfoToken(PP).namaToken, "do") != 0 && strcmp(InfoToken(PP).namaToken, "while") != 0 && strcmp(InfoToken(PP).namaToken, "for") != 0 && strcmp(InfoToken(PP).namaToken, "{") != 0 && strcmp(InfoToken(PP).namaToken, "}") != 0) {
			if (strcmp(InfoToken(PP).namaToken, "+") == 0 || strcmp(InfoToken(PP).namaToken, "-") == 0 || strcmp(InfoToken(PP).namaToken, "*") == 0 || strcmp(InfoToken(PP).namaToken, "=") == 0) {
				PPtemp = GetBefore(LT, PP);
				if (strcmp(InfoToken(PP).namaToken, "=") == 0) {
					if ((strcmp(InfoToken(PPtemp).namaToken, "(")==0) || (strcmp(InfoToken(PPtemp).namaToken, "begin")==0) || (strcmp(InfoToken(PPtemp).namaToken, "end")==0) || (strcmp(InfoToken(PPtemp).namaToken, "if")==0) || (strcmp(InfoToken(PPtemp).namaToken, "then")==0)) {
						printf("INVALID EXPRESSION at row %d, cannot find variabel before %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(PPtemp).namaToken);
						error = 1;
					}
					else if ((strcmp(InfoToken(PPtemp).namaToken, "*")==0) || (strcmp(InfoToken(PPtemp).namaToken, "-")==0) || (strcmp(InfoToken(PPtemp).namaToken, "+")==0) || (strcmp(InfoToken(PPtemp).namaToken, "=")==0) || (strcmp(InfoToken(PPtemp).namaToken, "input")==0) || (strcmp(InfoToken(PPtemp).namaToken, "output")==0) || (strcmp(InfoToken(PPtemp).namaToken, "NIL")==0)) {
						printf("INVALID EXPRESSION at row %d, cannot find variabel before %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(PPtemp).namaToken);
						error = 1;
					}
					else if ((strcmp(InfoToken(NextToken(PP)).namaToken, ">")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "output")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "input")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "then")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "if")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "begin")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "end")==0)) {
						printf("INVALID EXPRESSION at row %d, cannot find variabel after %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(NextToken(PP)).namaToken);
						error = 1;
					}
					else if ((strcmp(InfoToken(NextToken(PP)).namaToken, "<")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "=")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "-")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "+")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "NIL")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, ")")==0)) {
						printf("INVALID EXPRESSION at row %d, cannot find variabel after %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(NextToken(PP)).namaToken);
						error = 1;
					}
				}
				else {
					if ((strcmp(InfoToken(PPtemp).namaToken, "<")==0) || (strcmp(InfoToken(PPtemp).namaToken, "(")==0) || (strcmp(InfoToken(PPtemp).namaToken, "begin")==0) || (strcmp(InfoToken(PPtemp).namaToken, "end")==0) || (strcmp(InfoToken(PPtemp).namaToken, "if")==0) || (strcmp(InfoToken(PPtemp).namaToken, "then")==0)) {
						printf("INVALID EXPRESSION at row %d, cannot find variabel before %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(PPtemp).namaToken);
						error = 1;
					}
					else if ((strcmp(InfoToken(PPtemp).namaToken, ">")==0) || (strcmp(InfoToken(PPtemp).namaToken, "*")==0) || (strcmp(InfoToken(PPtemp).namaToken, "-")==0) || (strcmp(InfoToken(PPtemp).namaToken, "+")==0) || (strcmp(InfoToken(PPtemp).namaToken, "=")==0) || (strcmp(InfoToken(PPtemp).namaToken, "input")==0) || (strcmp(InfoToken(PPtemp).namaToken, "output")==0) || (strcmp(InfoToken(PPtemp).namaToken, "NIL")==0)) {
						printf("INVALID EXPRESSION at row %d, cannot find variabel before %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(PPtemp).namaToken);
						error = 1;
					}
					else if ((strcmp(InfoToken(NextToken(PP)).namaToken, ">")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "output")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "input")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "then")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "if")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "begin")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "end")==0)) {
						printf("INVALID EXPRESSION at row %d, cannot find variabel after %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(NextToken(PP)).namaToken);
						error = 1;
					}
					else if ((strcmp(InfoToken(NextToken(PP)).namaToken, "<")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "=")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "-")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "+")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "NIL")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, ")")==0)) {
						printf("INVALID EXPRESSION at row %d, cannot find variabel after %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(NextToken(PP)).namaToken);
						error = 1;
					}
				}
			}
			
			else if (strcmp(InfoToken(PP).namaToken, "<") == 0) {
				PPtemp = GetBefore(LT, PP);
				if ((strcmp(InfoToken(PPtemp).namaToken, "(")==0) || (strcmp(InfoToken(PPtemp).namaToken, "begin")==0) || (strcmp(InfoToken(PPtemp).namaToken, "end")==0) || (strcmp(InfoToken(PPtemp).namaToken, "if")==0) || (strcmp(InfoToken(PPtemp).namaToken, "then")==0)) {
					printf("INVALID EXPRESSION at row %d, cannot find variabel before %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(PPtemp).namaToken);
					error = 1;
				}
				else if ((strcmp(InfoToken(PPtemp).namaToken, ">")==0) || (strcmp(InfoToken(PPtemp).namaToken, "<")==0) || (strcmp(InfoToken(PPtemp).namaToken, "*")==0) || (strcmp(InfoToken(PPtemp).namaToken, "-")==0) || (strcmp(InfoToken(PPtemp).namaToken, "+")==0) || (strcmp(InfoToken(PPtemp).namaToken, "=")==0) || (strcmp(InfoToken(PPtemp).namaToken, "input")==0) || (strcmp(InfoToken(PPtemp).namaToken, "output")==0) || (strcmp(InfoToken(PPtemp).namaToken, "NIL")==0)) {
					printf("INVALID EXPRESSION at row %d, cannot find variabel before %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(PPtemp).namaToken);
					error = 1;
				}
				else if ((strcmp(InfoToken(NextToken(PP)).namaToken, "<")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "output")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "input")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "then")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "if")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "begin")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "end")==0)) {
					printf("INVALID EXPRESSION at row %d, cannot find variabel after %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(NextToken(PP)).namaToken);
					error = 1;
				}
				else if ((strcmp(InfoToken(NextToken(PP)).namaToken, "-")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "+")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "NIL")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, ")")==0)) {
					printf("INVALID EXPRESSION at row %d, cannot find variabel after %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(NextToken(PP)).namaToken);
					error = 1;
				}
			}
			
			else if (strcmp(InfoToken(PP).namaToken, ">") == 0) {
				PPtemp = GetBefore(LT, PP);
				if ((strcmp(InfoToken(PPtemp).namaToken, "(")==0) || (strcmp(InfoToken(PPtemp).namaToken, "begin")==0) || (strcmp(InfoToken(PPtemp).namaToken, "end")==0) || (strcmp(InfoToken(PPtemp).namaToken, "if")==0) || (strcmp(InfoToken(PPtemp).namaToken, "then")==0)) {
					printf("INVALID EXPRESSION at row %d, cannot find variabel before %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(PPtemp).namaToken);
					error = 1;
				}
				else if ((strcmp(InfoToken(PPtemp).namaToken, ">")==0) || (strcmp(InfoToken(PPtemp).namaToken, "*")==0) || (strcmp(InfoToken(PPtemp).namaToken, "-")==0) || (strcmp(InfoToken(PPtemp).namaToken, "+")==0) || (strcmp(InfoToken(PPtemp).namaToken, "=")==0) || (strcmp(InfoToken(PPtemp).namaToken, "input")==0) || (strcmp(InfoToken(PPtemp).namaToken, "output")==0) || (strcmp(InfoToken(PPtemp).namaToken, "NIL")==0)) {
					printf("INVALID EXPRESSION at row %d, cannot find variabel before %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(PPtemp).namaToken);
					error = 1;
				}
				else if ((strcmp(InfoToken(NextToken(PP)).namaToken, ">")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "output")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "input")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "then")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "if")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "begin")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "end")==0)) {
					printf("INVALID EXPRESSION at row %d, cannot find variabel after %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(NextToken(PP)).namaToken);
					error = 1;
				}
				else if ((strcmp(InfoToken(NextToken(PP)).namaToken, "<")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "-")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "+")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "NIL")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, ")")==0)) {
					printf("INVALID EXPRESSION at row %d, cannot find variabel after %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(NextToken(PP)).namaToken);
					error = 1;
				}
			}
			
			else if (strcmp(InfoToken(PP).namaToken, "to") == 0 && strcmp(s, "for") == 0) {
				PPtemp = GetBefore(LT, PP);
				if ((strcmp(InfoToken(PPtemp).namaToken, "(")==0) || (strcmp(InfoToken(PPtemp).namaToken, "begin")==0) || (strcmp(InfoToken(PPtemp).namaToken, "end")==0) || (strcmp(InfoToken(PPtemp).namaToken, "if")==0) || (strcmp(InfoToken(PPtemp).namaToken, "then")==0)) {
					printf("INVALID EXPRESSION at row %d, cannot find variabel before %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(PPtemp).namaToken);
					error = 1;
				}
				else if ((strcmp(InfoToken(PPtemp).namaToken, ">")==0) || (strcmp(InfoToken(PPtemp).namaToken, "*")==0) || (strcmp(InfoToken(PPtemp).namaToken, "-")==0) || (strcmp(InfoToken(PPtemp).namaToken, "+")==0) || (strcmp(InfoToken(PPtemp).namaToken, "=")==0) || (strcmp(InfoToken(PPtemp).namaToken, "input")==0) || (strcmp(InfoToken(PPtemp).namaToken, "output")==0) || (strcmp(InfoToken(PPtemp).namaToken, "NIL")==0)) {
					printf("INVALID EXPRESSION at row %d, cannot find variabel before %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(PPtemp).namaToken);
					error = 1;
				}
				else if ((strcmp(InfoToken(NextToken(PP)).namaToken, ">")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "output")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "input")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "then")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "if")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "begin")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "end")==0)) {
					printf("INVALID EXPRESSION at row %d, cannot find variabel after %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(NextToken(PP)).namaToken);
					error = 1;
				}
				else if ((strcmp(InfoToken(NextToken(PP)).namaToken, "<")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "-")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "+")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, "NIL")==0) || (strcmp(InfoToken(NextToken(PP)).namaToken, ")")==0)) {
					printf("INVALID EXPRESSION at row %d, cannot find variabel after %s, but %s\n", InfoToken(PP).barisToken, InfoToken(PP).namaToken, InfoToken(NextToken(PP)).namaToken);
					error = 1;
				}
			}
			PP = NextToken(PP);
		}
		}
		else {
			return 0;
		}
		
		*P = GetBefore(LT, PP);
		if (error == 1) {
			return 0;
		}
		else {
			return 1;
		}

	}
	else {
		printf("INVALID EXPRESSION at row %d, parenthesses problem\n", InfoToken(PP).barisToken);
		return 0;
	}
	/*
	PS = Top(ST);
	while (PS != Nil) {
		printf("stack %s\n", InfoStack(PS));
		PS = NextStack(PS);
	}
	*P = NextToken(*P);
	*/

	//return 1;
}

int IF(Address *P, char s[]) {
	Address temp, temp1, temp2, temp3;

	temp1 = GetBefore(LT, *P);
	if (strcmp(InfoToken(*P).namaToken, "(") == 0) {
		*P = NextToken(*P);
		if (*P != Nil) {
		if (EXP(P, "if") == 1) {
			if (strcmp(InfoToken(*P).namaToken, ")") == 0) {
				*P = NextToken(*P);
				if (strcmp(InfoToken(*P).namaToken, "then") == 0) {
					//printf("VALID whole (EXP) after IF from row %d\n", InfoToken(temp1).barisToken);
					temp=*P;
					*P = NextToken(*P);
					//printf("finding %s %d\n", InfoToken(*P).namaToken, InfoToken(*P).barisToken);
					if (*P != Nil) {
						if (START(P, "if")) {
							
							printf("VALID STARTPOINT after IF from row %d\n", InfoToken(temp1).barisToken);
							printf("NOW SYM %s %d\n", InfoToken(*P).namaToken, InfoToken(*P).barisToken);
							printf("PREV SYM %s %d\n", InfoToken(GetBefore(LT, *P)).namaToken, InfoToken(GetBefore(LT, *P)).barisToken);
							printf("NOW SYM %s %d\n", InfoToken(NextToken(*P)).namaToken, InfoToken(NextToken(*P)).barisToken);
							
							
							temp2 = *P;
							//*P = NextToken(*P);
							//if (*P != Nil) {
							if (NextToken(*P) != Nil) {
								if (strcmp(InfoToken(NextToken(*P)).namaToken, "else") == 0) {
									*P = NextToken(*P);
									printf("find else at row %d next %s\n", InfoToken(*P).barisToken, InfoToken(NextToken(*P)).namaToken);
									temp3 = *P;
									//*P = NextToken(*P);
									//if (*P != Nil) {
									if (NextToken(*P) != Nil) {
										*P = NextToken(*P);
										printf("NOW SYM %s %d\n", InfoToken(*P).namaToken, InfoToken(*P).barisToken);
										if (START(P, "if") == 1) {
											printf("VALID STARTPOINT after ELSE from row %d\n", InfoToken(temp3).barisToken);
											return 1;
										}
										else {
											printf("INVALID STARTPOINT after ELSE from row %d\n", InfoToken(temp3).barisToken);
											return 0;
										}
									}
									else {
										printf("FOUND NULL ADDRESS after ELSE from row %d\n", InfoToken(temp3).barisToken);
										return 0;
									}
								}
								else {
									counter=0;
									if (BLOCK(P, s) == 1) {
										//printf("VALID BLOCK after IF row %d AND END row %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
										return 1;
									}
									else {
										printf("INVALID BLOCK after IF row %d AND END row %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
										return 0;
									}
								}
							}
							else {
								printf("FOUND NULL ADDRESS after IF row %d AND END row %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
								return 0;
							}
						}
						else {
							printf("INVALID STARTPOINT after IF from row %d\n", InfoToken(temp1).barisToken);
							return 0;
						}
					}
					else {
						printf("FOUND NULL ADDRESS AFTER IF row %d\n", InfoToken(temp1).barisToken);
						return 0;
					}
				}
				else {
					printf("INVALID (EXP) for IF from row %d, expected then\n", InfoToken(temp1).barisToken);
					return 0;
				}
			}
			else {
				printf("INVALID (EXP) for IF from row %d, expected ) but %s\n", InfoToken(temp1).barisToken, InfoToken(*P).namaToken);
				return 0;
			}
		}
		else {
			printf("INVALID (EXP) for IF from row %d after (\n", InfoToken(temp1).barisToken);
			return 0;
		}
		}
		else {
			printf ("FOUND NULL ADDRESS after IF from row %d\n", InfoToken(temp1).barisToken);
			return 0;
		}
	}
	else {
		printf("INVALID (EXP) for IF from row %d, expected (\n", InfoToken(temp1).barisToken);
		return 0;
	}
}

int INPUT(Address *P, char s[]) {
	Address temp, temp1;
	temp = GetBefore(LT, *P);
	if (strcmp(InfoToken(*P).namaToken, "(") == 0) {
		*P = NextToken(*P);
		if (*P != Nil) {
			if (EXP(P, "input") == 1) {
				if (strcmp(InfoToken(*P).namaToken, ")") == 0) {
					//printf("VALID (EXP) for INPUT from row %d\n", InfoToken(temp).barisToken);
					temp1 = *P;
					//*P = NextToken(*P);
					//if (*P != Nil) {
					if (NextToken(*P) != Nil) {
						if (strcmp(InfoToken(NextToken(*P)).namaToken, "end") != 0) {
							*P = NextToken(*P);
						}
						if (BLOCK(P, s) == 1) {
							//printf("VALID BLOCK after INPUT row %d and ) row %d\n", InfoToken(temp).barisToken, InfoToken(temp1).barisToken);
							return 1;
						}
						else {
							printf("INVALID BLOCK after INPUT row %d and ) row %d\n", InfoToken(temp).barisToken, InfoToken(temp1).barisToken);
							return 0;
						}
					}
					else {
						printf("FOUND NULL ADDRESS after INPUT row %d and ) row %d\n", InfoToken(temp).barisToken, InfoToken(temp1).barisToken);
						return 0;
					}
				}
				else {
					printf("INVALD (EXP) for INPUT from row %d\n", InfoToken(temp).barisToken);
					return 0;
				}
			}
			else {
				printf("INVALID (EXP) for INPUT from row %d\n", InfoToken(temp).barisToken);
				return 0;
			}
		}
		else {
			printf("FOUND NULL ADDRESS after INPUT from row %d\n", InfoToken(temp).barisToken);
			return 0;
		}
	}
	else {
		printf("INVALID (EXP) after INPUT from row %d\n", InfoToken(temp).barisToken);
		return 0;
	}
}

int OUTPUT(Address *P, char s[]) {
	Address temp, temp1;
	temp = GetBefore(LT, *P);
	if (strcmp(InfoToken(*P).namaToken, "(") == 0) {
		*P = NextToken(*P);
		if (*P != Nil) {
			if (EXP(P, "output") == 1) {
				if (strcmp(InfoToken(*P).namaToken, ")") == 0) {
					//printf("VALID (EXP) for OUTPUT from row %d\n", InfoToken(temp).barisToken);
					temp1 = *P;
					//*P = NextToken(*P);
					//if (*P != Nil) {
					if (NextToken(*P) != Nil) {
						if (strcmp(InfoToken(NextToken(*P)).namaToken, "end") != 0) {
							*P = NextToken(*P);
						}
						if (BLOCK(P, s) == 1) {
							//printf("VALID BLOCK after OUTPUT row %d and ) row %d\n", InfoToken(temp).barisToken, InfoToken(temp1).barisToken);
							return 1;
						}
						else {
							printf("INVALID BLOCK after OUTPUT row %d and ) row %d\n", InfoToken(temp).barisToken, InfoToken(temp1).barisToken);
							return 0;
						}
					}
					else {
						printf("FOUND NULL ADDRESS after OUTPUT row %d and ) row %d\n", InfoToken(temp).barisToken, InfoToken(temp1).barisToken);
						return 0;
					}
				}
				else {
					printf("INVALD (EXP) for OUTPUT from row %d\n", InfoToken(temp).barisToken);
					return 0;
				}
			}
			else {
				printf("INVALID (EXP) for OUTPUT from row %d\n", InfoToken(temp).barisToken);
				return 0;
			}
		}
		else {
			printf("FOUND NULL ADDRESS after OUTPUT from row %d\n", InfoToken(temp).barisToken);
			return 0;
		}
	}
	else {
		printf("INVALID (EXP) after OUTPUT from row %d\n", InfoToken(temp).barisToken);
		return 0;
	}
}

int DO(Address *P, char s[]) {
	Address temp, temp1, temp2, temp3;
	temp = *P;
	if (strcmp(InfoToken(*P).namaToken, "do") == 0) {
		if (strcmp(InfoToken(NextToken(*P)).namaToken, "end") != 0) {
			if (strcmp(InfoToken(NextToken(*P)).namaToken, "while") != 0) {
				*P = NextToken(*P);
				/*
				if (strcmp(InfoToken(*P).namaToken, "begin") == 0) {
					printf("INVALID EXP after DO from row %d, cannot be BEGIN after DO\n", InfoToken(temp).barisToken);
					return 0;
				}
				*/
			}
		}
		if (BLOCK(P, "do") == 1) {
			*P = NextToken(*P);
			if (*P != Nil) {
				temp3 = *P;
				if (strcmp(InfoToken(*P).namaToken, "while") == 0) {
					*P = NextToken(*P);
					if (*P != Nil) {
						if (strcmp(InfoToken(*P).namaToken, "(") == 0) {
							*P = NextToken(*P);
							if (*P != Nil) {
								if (EXP(P, "while") == 1) {
									if (strcmp(InfoToken(*P).namaToken, ")") == 0) {
										printf("VALID DO-WHILE from begin row %d and end row %d\n", InfoToken(temp).barisToken, InfoToken(*P).barisToken);
										temp1 = *P;
										//*P = NextToken(*P);
										//temp2 = *P;
										temp2 = NextToken(*P);

										//if (*P != Nil) {
										if (NextToken(*P) != Nil) {
											//printf("cek %s\n", InfoToken(NextToken(*P)).namaToken);
											if (strcmp(InfoToken(NextToken(*P)).namaToken, "end") != 0) {
												if (strcmp(InfoToken(NextToken(*P)).namaToken, "else") != 0) {
													if (strcmp(InfoToken(NextToken(*P)).namaToken, "while") != 0) {
														*P = NextToken(*P);
													}
												}
											}
							
											printf("CEKW %s %d\n", InfoToken(*P).namaToken, InfoToken(*P).barisToken);
											if (strcmp(InfoToken(NextToken(*P)).namaToken, "else") == 0) {
												if (strcmp(s, "if")==0) {
													return 1;
												}
												else {
													printf("INVALID ELSE at row %d, expected IF before ELSE\n", InfoToken(NextToken(*P)).barisToken);
													return 0;
												}
											}
											else {
												if (BLOCK(P, s) == 1) {
													//printf("VALID BLOCK from row %d till %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
													return 1;
												}
												else {
													printf("INVALID BLOCK AFTER WHILE from row %d\n", InfoToken(temp1).barisToken);
													return 0;
												}
											}
										}
										else {
											return 1;
										}
									}
									else {
										printf("INVALID (EXP) after WHILE from row %d, expected ) in the end, but %s\n", InfoToken(temp3).barisToken, InfoToken(*P).namaToken);
										return 0;
									}
								}
								else {
									printf("INVALID WHOLE (EXP) after WHILE from row %d\n", InfoToken(temp3).barisToken);
									return 0;
								}
							}
							else {
								printf("INVALID (EXP) after ( from row %d\n", InfoToken(temp3).barisToken);
								return 0;
							}
						}
						else {
							printf("INVALID (EXP) after WHILE from row %d, expected (\n", InfoToken(*P).barisToken);
							return 0;
						}
					}
					else {
						printf("FOUND NULL-ADDRESS after WHILE from row %d, expected (\n", InfoToken(*P).barisToken);
						return 0;
					}
				}
				else {
					printf("INVALID after DO from row %d, expected WHILE after DO\n", InfoToken(GetBefore(LT, *P)).barisToken);
					return 0;
				}
			}
			else {
				printf("FOUND NULL-ADDRESS after DO from row %d, expected WHILE after DO\n", InfoToken(GetBefore(LT, *P)).barisToken);
				return 0;
			}
		}
		else {
			printf("INVALID BLOCK after DO from row %d\n", InfoToken(*P).barisToken);
			return 0;
		}
	}
	else {
		*P = NextToken(*P);
		if (*P != Nil) {
			return DO(P, s);
		}
		else {
			printf("INVALID DO, not found DO first time %d, expected DO\n", InfoToken(GetPrev(LT)).barisToken);
		}
		return 0;
	}
}

int FOR(Address *P, char s[]) {
	Address temp, temp1, temp2, temp3;

	temp1 = GetBefore(LT, *P);
	if (strcmp(InfoToken(*P).namaToken, "(") == 0) {
		*P = NextToken(*P);
		if (*P != Nil) {
		if (EXP(P, "for") == 1) {
			if (strcmp(InfoToken(*P).namaToken, ")") == 0) {
				*P = NextToken(*P);
				if (strcmp(InfoToken(*P).namaToken, "do") == 0) {
					//printf("VALID whole (EXP) after FOR from row %d\n", InfoToken(temp1).barisToken);
					temp=*P;
					*P = NextToken(*P);
					//printf("finding %s %d\n", InfoToken(*P).namaToken, InfoToken(*P).barisToken);
					if (*P != Nil) {
						if (START(P, "for")) {
							/*
							printf("VALID STARTPOINT after FOR from row %d\n", InfoToken(temp1).barisToken);
							printf("NOW SYM %s %d\n", InfoToken(*P).namaToken, InfoToken(*P).barisToken);
							printf("PREV SYM %s %d\n", InfoToken(GetBefore(LT, *P)).namaToken, InfoToken(GetBefore(LT, *P)).barisToken);
							printf("NOW SYM %s %d\n", InfoToken(NextToken(*P)).namaToken, InfoToken(NextToken(*P)).barisToken);
							*/
							
							temp2 = *P;
							//*P = NextToken(*P);
							//if (*P != Nil) {
							if (NextToken(*P) != Nil) {
								
								if (strcmp(InfoToken(NextToken(*P)).namaToken, "end") != 0) {
									if (strcmp(InfoToken(NextToken(*P)).namaToken, "else") != 0) {
										if (strcmp(InfoToken(NextToken(*P)).namaToken, "while") != 0) {
											*P = NextToken(*P);
										}
									}
								}
							
								printf("CEKF %s %d\n", InfoToken(*P).namaToken, InfoToken(*P).barisToken);
								if (strcmp(InfoToken(NextToken(*P)).namaToken, "else") == 0) {
									if (strcmp(s, "if")==0) {
										return 1;
									}
									else {
										printf("INVALID ELSE at row %d, expected IF before ELSE\n", InfoToken(NextToken(*P)).barisToken);
										return 0;
									}
								}
								else {
									if (BLOCK(P, s) == 1) {
										//printf("VALID BLOCK from row %d till %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
										return 1;
									}
									else {
										printf("INVALID BLOCK AFTER FOR from row %d\n", InfoToken(temp1).barisToken);
										return 0;
									}
								}
							}
							else {
								printf("FOUND NULL ADDRESS after FOR row %d AND END row %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
								return 0;
							}
						}
						else {
							printf("INVALID STARTPOINT after FOR from row %d\n", InfoToken(temp1).barisToken);
							return 0;
						}
					}
					else {
						printf("FOUND NULL ADDRESS AFTER FOR row %d\n", InfoToken(temp1).barisToken);
						return 0;
					}
				}
				else {
					printf("INVALID (EXP) for FOR from row %d, expected do\n", InfoToken(temp1).barisToken);
					return 0;
				}
			}
			else {
				printf("INVALID (EXP) for FOR from row %d, expected ) but %s\n", InfoToken(temp1).barisToken, InfoToken(*P).namaToken);
				return 0;
			}
		}
		else {
			printf("INVALID (EXP) for FOR from row %d after (\n", InfoToken(temp1).barisToken);
			return 0;
		}
		}
		else {
			printf ("FOUND NULL ADDRESS after FOR from row %d\n", InfoToken(temp1).barisToken);
			return 0;
		}
	}
	else {
		printf("INVALID (EXP) after FOR from row %d, expected (\n", InfoToken(temp1).barisToken);
		return 0;
	}
}

int BLOCK(Address *P, char s[]) {
	Address temp, temp1, temp2;

	temp1 = *P;
	if (strcmp(InfoToken(*P).namaToken, "if") == 0) {
		*P = NextToken(*P);
		if (*P != Nil) {
			temp = *P;
			//printf("found if %d\n", InfoToken(*P).barisToken);
			if (IF(P, s) == 1) {
				//printf("VALID IF from row %d to row %d\n", InfoToken(temp1).barisToken, InfoToken(*P).barisToken);
				temp2 = *P;
				//*P = NextToken(*P);
				//if (*P == Nil) {
				if (NextToken(*P) == Nil) {
					printf("FOUND NULL ADDRESS after IF row %d AND END row %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
					return 0;
				}
				else {
					if (strcmp(InfoToken(NextToken(*P)).namaToken, "end")!=0) {
						if (strcmp(InfoToken(NextToken(*P)).namaToken, "while") != 0) {
							if (strcmp(InfoToken(NextToken(*P)).namaToken, "else") != 0) {
								*P = NextToken(*P);
							}
						}
					}
					if (BLOCK(P, s) == 1) {
						//printf("VALID BLOCK after IF row %d AND END row %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
						return 1;
					}
					else {
						printf("INVALID BLOCK after IF row %d AND END row %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
						return 0;
					}
				}
				//return BLOCK(P, s);
			}
			else {
				printf("INVALID EXP for IF from row %d\n", InfoToken(temp1).barisToken);
				return 0;
			}
		}
		else {
			printf("INVALID AFTER IF from row %d, expected (\n", InfoToken(temp1).barisToken);
			return 0;
		}
	}

	else if (strcmp(InfoToken(*P).namaToken, "input") == 0) {
		Address temp, temp1, temp2;

		temp = *P;
		*P = NextToken(*P);
		if (*P != Nil) {
			temp1 = *P;
			if (INPUT(P, s) == 1) {
				printf("VALID INPUT from row %d\n", InfoToken(temp).barisToken);
				temp2 = *P;
				if (NextToken(*P) != Nil) {
					if (strcmp(InfoToken((NextToken(*P))).namaToken, "end") != 0) {
						if (strcmp(InfoToken((NextToken(*P))).namaToken, "while") != 0) {
							*P = NextToken(*P);
						}
					}
					//printf("NOWBRO %s %d\n", InfoToken(*P).namaToken, InfoToken(*P).barisToken);
					if (BLOCK(P, s) == 1) {
						//printf("VALID BLOCK after INPUT from row %d\n", InfoToken(temp).barisToken);
						return 1;
					}
					else {
						printf("INVALID BLOCK after INPUT from row %d\n", InfoToken(temp).barisToken);
						return 0;
					}
				}
				else {
					printf("FOUND NULL ADDRESS after INPUT from row %d\n", InfoToken(temp2).barisToken);
					return 0;
				}
			}
			else {
				printf("INVALID EXP for INPUT from row %d, expected (\n", InfoToken(temp).barisToken);
				return 0;
			}
		}
		else {
			printf("FOUND NULL ADDRESS after INPUT from row %d, expected (", InfoToken(temp).barisToken);
			return 0;
		}
	}

	else if (strcmp(InfoToken(*P).namaToken, "output") == 0) {
		Address temp, temp1, temp2;

		temp = *P;
		*P = NextToken(*P);
		if (*P != Nil) {
			temp1 = *P;
			if (OUTPUT(P, s) == 1) {
				//printf("VALID OUTPUT from row %d\n", InfoToken(temp).barisToken);
				temp2 = *P;
				if (NextToken(*P) != Nil) {
					if (strcmp(InfoToken((NextToken(*P))).namaToken, "end") != 0) {
						*P = NextToken(*P);
					}
					if (BLOCK(P, s) == 1) {
						//printf("VALID BLOCK after OUTPUT from row %d\n", InfoToken(temp).barisToken);
						return 1;
					}
					else {
						printf("INVALID BLOCK after OUTPUT from row %d\n", InfoToken(temp).barisToken);
						return 0;
					}
				}
				else {
					printf("FOUND NULL ADDRESS after OUTPUT from row %d\n", InfoToken(temp2).barisToken);
					return 0;
				}
			}
			else {
				printf("INVALID EXP for OUTPUT from row %d, expected (\n", InfoToken(temp).barisToken);
				return 0;
			}
		}
		else {
			printf("FOUND NULL ADDRESS after OUTPUT from row %d, expected (", InfoToken(temp).barisToken);
			return 0;
		}
	}

	/*
	else if (strcmp(InfoToken(*P).namaToken, "else") == 0) {
		printf("INVALID at row %d, expected IF before ELSE\n", InfoToken(*P).barisToken);
		return 0;
	}
	*/
	
	else if (strcmp(InfoToken(*P).namaToken, "do") == 0) {
		Address temp;
		temp = *P;
		printf("FOUND DO at row %d\n", InfoToken(*P).barisToken);
		if (DO(P, "do") == 1) {
			//*P = NextToken(*P);
			if (BLOCK(P, "do") == 1) {
				printf("VALID!!!\n");
				return 1;
			}
			else {
				printf("INVALID BLOCK after DO from row %d\n", InfoToken(temp).barisToken);
				return 0;
			}
		}
		else {
			printf("INVALID DO-WHILE from row %d\n", InfoToken(*P).barisToken);
			return 0;
		}
	}
	
	else if (strcmp(InfoToken(*P).namaToken, "for") == 0) {
		*P = NextToken(*P);
		if (*P != Nil) {
			temp = *P;
		
			//printf("found if %d\n", InfoToken(*P).barisToken);
			if (FOR(P, "for") == 1) {
				//printf("VALID IF from row %d to row %d\n", InfoToken(temp1).barisToken, InfoToken(*P).barisToken);
				temp2 = *P;
				//*P = NextToken(*P);
				//if (*P == Nil) {
				if (NextToken(*P) == Nil) {
					printf("FOUND NULL ADDRESS after FOR row %d AND END row %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
					return 0;
				}
				else {
					if (strcmp(InfoToken(NextToken(*P)).namaToken, "end")!=0) {
						if (strcmp(InfoToken(NextToken(*P)).namaToken, "while") != 0) {
							if (strcmp(InfoToken(NextToken(*P)).namaToken, "else") != 0) {
								*P = NextToken(*P);
							}
						}
					}
					if (BLOCK(P, s) == 1) {
						//printf("VALID BLOCK after FOR row %d AND END row %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
						return 1;
					}
					else {
						printf("INVALID BLOCK after FOR row %d AND END row %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
						return 0;
					}
				}
				//return BLOCK(P, s);
			}
			else {
				printf("INVALID EXP for FOR from row %d\n", InfoToken(temp1).barisToken);
				return 0;
			}
		}
		else {
			printf("INVALID AFTER FOR from row %d, expected ( but NULL-ADDRESS\n", InfoToken(temp1).barisToken);
			return 0;
		}
	}
	
	/*
	else if (strcmp(InfoToken(*P).namaToken, "begin") == 0) {
		printf("FOUND B E G I N row %d\n", InfoToken(*P).barisToken);
		if (strcmp(s, "do") == 0) {
			printf("INVALID EXP after DO, cannot be BEGIN at row %d\n", InfoToken(*P).barisToken);
			return 0;
		}
	}
	*/
	
	else {
		if (strcmp(InfoToken(NextToken(*P)).namaToken, "end") == 0) {
			//printf("FOUND END row %d\n", InfoToken(NextToken(*P)).barisToken);
			return 1;
		}
		else if (strcmp(InfoToken(NextToken(*P)).namaToken, "while") == 0) {
			printf("FOUND WHILE row %d\n", InfoToken(NextToken(*P)).barisToken);
			//*P = NextToken(*P);
			/*
			if (strcmp(s, "do") == 0) {
				return 1;
			}
			else {
				printf("INVALID WHILE at row %d, expected DO before WHILE\n", InfoToken(NextToken(*P)).barisToken);
				return 0;
			}
			*/
			int countw = 0, countd = 0, foundd = 0;
			Address AT;
			AT = *P;
			while (AT != FirstToken(LT)) {
				if (strcmp(InfoToken(AT).namaToken, "while") == 0) {
					countw++;
				}
				else if (strcmp(InfoToken(AT).namaToken, "do") == 0) {
					if (strcmp(InfoToken(NextToken(AT)).namaToken, "begin") != 0) {
						countd++;
						if (countd == countw+1) {
							printf("found yes do row %d\n", InfoToken(AT).barisToken);
							foundd = 1;
							break;
						}
					}
				}
				AT = GetBefore(LT, AT);
			}
			if (foundd == 1) {
				return 1;
			}
			else {
				printf("INVALID WHILE at row %d, expected DO before WHILE\n", InfoToken(NextToken(*P)).barisToken);
				return 0;
			}
		}
		else if (strcmp(InfoToken(NextToken(*P)).namaToken, "else") == 0) {
			if (strcmp(s, "if") == 0) {
				return 1;
			}
			else {
				printf("INVALID ELSE at row %d, expected IF before ELSE\n", InfoToken(NextToken(*P)).barisToken);
				return 0;
			}
		}
		else if (strcmp(InfoToken(*P).namaToken, "begin") != 0) {
			if (strcmp(InfoToken(*P).namaToken, "end") != 0) {
				if (strcmp(InfoToken(*P).namaToken, "if") != 0) {
					if (strcmp(InfoToken(*P).namaToken, "then") != 0) {
						if (strcmp(InfoToken(*P).namaToken, "else") != 0) {
							if (strcmp(InfoToken(*P).namaToken, "while") !=0) {
								if (EXP(&NextToken(*P), "exp") == 1) {
									//printf("VALIDS EXPRESSION at row %d\n", InfoToken(*P).barisToken);
									*P = NextToken(*P);
									//*P = NextToken(*P);
									if (strcmp(InfoToken(NextToken(*P)).namaToken, "end") != 0) {
										if (strcmp(InfoToken(NextToken(*P)).namaToken, "while") != 0) {
											if (strcmp(InfoToken(NextToken(*P)).namaToken, "else") == 0) {
												//*P = NextToken(*P);
												return BLOCK(P, "exp"); 
											}
											else {
												*P = NextToken(*P);
												return BLOCK(P, s);
											}
										}
										else {
											return BLOCK(P, s);
										}
									}
									else {
										//printf("NOW %s\n", InfoToken(*P).namaToken);
										return BLOCK(P, s);
									}
									//return 1;
								}
								else {
									printf("INVALID EXPRESSION at row %d\n", InfoToken(*P).barisToken);
									//*P = NextToken(*P);
									return 0;
								}
							}
						}
					}
				}
			}
		}
		else {
			*P = NextToken(*P);
			if (*P != Nil) {
			//if (NextToken(*P) != Nil) {
				return BLOCK(P, s);
			}
			else {
				Address PP;
				PP = GetPrev(LT);

				if (strcmp(s, "begin") == 0) {
					printf("INVALID at row %d, expected END\n", InfoToken(GetPrev(LT)).barisToken);
				}
				return 0;
			}
		}
	}
}

int START(Address *P, char s[]) {
	//puts("start\n");
	Address temp, temp1, temp2;
	if (strcmp(InfoToken(*P).namaToken, "begin") == 0) {
		//printf("FOUND begin at row %d\n", InfoToken(*P).barisToken);
		temp = *P;
		//*P = NextToken(*P);
		if (*P != Nil) {

			if (BLOCK(P, "begin")) {
				//printf("VALID BLOCK after BEGIN from row %d\n", InfoToken(temp).barisToken);
				*P = NextToken(*P);
				if (*P != Nil) {
					if (strcmp(InfoToken(*P).namaToken, "end") == 0) {
						//printf("VALID BEGIN-END from begin row %d and end row %d\n", InfoToken(temp).barisToken, InfoToken(*P).barisToken);
						temp1 = *P;
						//*P = NextToken(*P);
						//temp2 = *P;
						temp2 = NextToken(*P);

						//if (*P != Nil) {
						if (NextToken(*P) != Nil) {
							//printf("cek %s\n", InfoToken(NextToken(*P)).namaToken);
							if (strcmp(InfoToken(NextToken(*P)).namaToken, "end") != 0) {
								if (strcmp(InfoToken(NextToken(*P)).namaToken, "else") != 0) {
									if (strcmp(InfoToken(NextToken(*P)).namaToken, "while") != 0) {
										*P = NextToken(*P);
									}
								}
							}
							
							printf("CEK %s %d\n", InfoToken(*P).namaToken, InfoToken(*P).barisToken);
							if (strcmp(InfoToken(NextToken(*P)).namaToken, "else") == 0) {
								if (strcmp(s, "if")==0) {
									return 1;
								}
								else {
									printf("INVALID ELSE at row %d, expected IF before ELSE\n", InfoToken(NextToken(*P)).barisToken);
									return 0;
								}
							}
							else {
								if (BLOCK(P, s) == 1) {
									printf("VALID BLOCK from row %d till %d\n", InfoToken(temp1).barisToken, InfoToken(temp2).barisToken);
									return 1;
								}
								else {
									printf("INVALID BLOCK AFTER END from row %d\n", InfoToken(temp1).barisToken);
									return 0;
								}
							}
						}
						else {
							return 1;
						}
					}
					else {
						printf("INVALID from BEGIN row %d, expected END at row %d\n", InfoToken(temp).barisToken, InfoToken(*P).barisToken);
						return 0;
					}
				}
				else {
					printf("INVALID after BEGIN, found NULL ADDRESS, expected END\n");
					return 0;
				}
			}
			else {
				printf("INVALID BLOCK after BEGIN from row %d\n", InfoToken(temp).barisToken);
				return 0;
			}
		}
		else {
			printf("INVALID after BEGIN from %d , expected END\n", InfoToken(temp).barisToken);
			return 0;
		}
	}
	else {
		*P = NextToken(*P);
		if (*P != Nil) {
			if (strcmp(InfoToken(*P).namaToken, "begin") != 0) {
				printf("INVALID identifier, expected BEGIN at row %d\n", InfoToken(*P).barisToken);
				return 0;
			}
			else {
				return START(P, s);
			}
		}
		else {
			printf("INVALID start, not found BEGIN first time %d, expected BEGIN\n", InfoToken(GetPrev(LT)).barisToken);
		}
		return 0;
	}
}

int main() {
	Address FA;
	int validity;
	/** Membuat list kosong untuk token **/
	CreateList (&LT);

	BacaToken ();

	PrintListToken(LT);

	/** Memulai membaca program **/
	if (IsListEmpty(LT)) {
		printf("PROGRAM Valid\n");
	}
	else {
		FA = FirstToken(LT);
		validity = START(&FA, "main");
	}

	printf("Validitas program: %d\n", validity);

	Address AD, temp;
	AD = FirstToken(LT);
	while (AD != Nil) {
		temp = AD;
		Dealokasi(&AD);
		AD = NextToken(temp);

	}
	//getchar();
	return 0;
}
