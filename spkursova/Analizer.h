#pragma once
Lexema* GetNextLexem(FILE* f, int ii);	//get next tokens from input file
void PrintLexemsInFile();	// print all tokens in file
int AnalisisLexems(FILE* FIn); // analisis number of tokens
// Return count of tokens
int AnalisisLexems(FILE* FIn)
{
	Lexema* TempLexema;
	int i = 0;		//count tokens
	char type[50];
	strcpy(type, "");

	do
	{
		TempLexema = GetNextLexem(FIn, i);
		strncpy(Data.LexTable[i].name, TempLexema->name, 49);
		Data.LexTable[i].value = TempLexema->value;
		Data.LexTable[i].type = TempLexema->type;
		Data.LexTable[i].line = TempLexema->line;
		i++;
	} while (TempLexema->type != LEOF);
	return i;
}

Lexema* GetNextLexem(FILE* f, int ii)
{
	char ch, buf[50];
	bool IsComment = false;
	Lexema* res;
	static int line = 1;
	res = (Lexema*)calloc(1, sizeof(Lexema));
	for (;;)
	{
		ch = getc(f);
		if (ch == '{')
		{
			char c;
			c = getc(f);
			if (c == '*')
			{
				while (getc(f) != '*')
				{
				}
				if (getc(f) != '}')
				{
					strncpy(res->name, "{*...*", 7);
					res->type = LUnknown;
					res->value = 0;
					res->line = line;
					ungetc(c, f);
					break;
				}
			}
			else
			{
				strncpy(res->name, "{", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				ungetc(c, f);
				break;
			}
		}
		else if (ch == '\n') line++;
		else if (ch == EOF)
		{
			strncpy(res->name, "EOF", 4);
			res->type = LEOF;
			res->value = 0;
			res->line = line;
			break;
		}

		else if (ch == '(')
		{
			strncpy(res->name, "(", 2);
			res->type = LLBraket;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == ')')
		{
			strncpy(res->name, ")", 2);
			res->type = LRBraket;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == ';')
		{
			strncpy(res->name, ";", 2);
			res->type = LSeparator;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == ',')
		{
			strncpy(res->name, ",", 2);
			res->type = LComma;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '-')				//відємне число
		{
			char c = getc(f);
			if (isdigit(c))
			{
				int i = 2;
				buf[0] = ch;
				buf[1] = c;
				for (; isdigit(ch = getc(f)); ++i)
				{
					buf[i] = ch;
				}
				ungetc(ch, f);
				buf[i] = '\0';
				strncpy(res->name, buf, i);
				res->type = LNumber;
				res->value = atoi(buf);
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "-", 2);
				res->type = LSub;
				res->value = 0;
				res->line = line;
				ungetc(c, f);
				break;
			}

		}
		else if (isdigit(ch))				//додатнє число
		{
			int i = 1;
			buf[0] = ch;
			for (; isdigit(ch = getc(f)); ++i)
			{
				buf[i] = ch;
			}
			ungetc(ch, f);
			buf[i] = '\0';
			strncpy(res->name, buf, i);
			res->type = LNumber;
			res->value = atoi(buf);
			res->line = line;
			break;
		}
		//**********************************************
		else if (ch == '+')
		{
			strncpy(res->name, "+", 2);
			res->type = LAdd;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '*')
		{
			strncpy(res->name, "*", 2);
			res->type = LMul;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '#')
		{
			strncpy(res->name, "#", 2);
			res->type = LBegProg;
			res->value = 0;
			res->line = line;
			break;
		}
		else if (ch == '>')
		{
			char c;
			c = getc(f);
			if (c == '=')
			{
				strncpy(res->name, ">=", 3);
				res->type = LGreate;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "=", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				ungetc(c, f);
				break;
			}
		}
		else if (ch == '!')
		{
			char c;
			c = getc(f);
			if (c == '!')
			{
				strncpy(res->name, "!!", 3);
				res->type = LNot;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "!", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				ungetc(c, f);
				break;
			}
		}
		else if (ch == '<')
		{
			char c = getc(f);
			if (isalpha(c))
			{
				//buf[0] = ch;
				buf[0] = c;
				int i;
				for (i = 1;; ++i)
				{
					ch = getc(f);
					if (ch != '>') buf[i] = ch;
					else break;
				}
				//buf[++i] = ch;
				ch = getc(f);
				ungetc(ch, f);
				buf[i] = '\0';
				strncpy(res->name, buf, i);
				res->type = LProgramName;
				res->value = 0;
				res->line = line;
				break;
			}
			else if (c == '-')
			{
				strncpy(res->name, "<-", 3);
				res->type = LNewValue;
				res->value = 0;
				res->line = line;
				break;
			}
			else if (c == '=')
			{
				strncpy(res->name, "<=", 3);
				res->type = LGreate;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, "<", 2);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				ungetc(c, f);
				break;
			}
		}

		else if (isalpha(ch) || ch == '_')               //Зарезервоване слово або ідентифікатор
		{
			int i = 0;
			buf[0] = ch;
			for (i = 1;; ++i)
			{
				ch = getc(f);
				if ((isdigit(ch) != 0) || (isalpha(ch) != 0) || ch == '_') buf[i] = ch;
				else break;
			}
			int j;
			for (j = 1; j < i; j++)
			{
				if (isupper(buf[j])) break;
			}
			ungetc(ch, f);
			buf[i] = '\0';
			strncpy(res->name, buf, i);

			//співставлення ідентифікаторів та зарезервованих слів
			if (strcmp(buf, "Program") == 0)
			{
				res->type = LProgram;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Start")) == 0)
			{
				res->type = LBody;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Variable")) == 0)
			{
				res->type = LData;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Int_2")) == 0)
			{
				res->type = LVarType;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Stop")) == 0)
			{
				res->type = LEnd;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Read")) == 0)
			{
				res->type = LScan;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Write")) == 0)
			{
				res->type = LPrint;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "While")) == 0)
			{
				res->type = LWhile;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Do")) == 0)
			{
				res->type = LDo;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "And")) == 0)
			{
				res->type = LAnd;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Or")) == 0)
			{
				res->type = LOr;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Eq")) == 0)
			{
				res->type = LEqu;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Ne")) == 0)
			{
				res->type = LNotEqu;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Div")) == 0)
			{
				res->type = LDiv;
				res->line = line;
				break;
			}
			else if ((strcmp(buf, "Mod")) == 0)
			{
				res->type = LMod;
				res->line = line;
				break;
			}
			else if ((j == i) && (i <= 4))
			{
				/*buf[i + 1] = buf[i];
				buf[i] = '_';
				i++;*/
				strncpy(res->name, buf, i);
				res->type = LIdentifier;
				res->value = 0;
				res->line = line;
				break;
			}
			else
			{
				strncpy(res->name, buf, i);
				res->type = LUnknown;
				res->value = 0;
				res->line = line;
				break;
			}
			res->value = 0;
			res->line = line;
		}
		else if ((ch != '\n') && (ch != '\t') && (ch != ' ')/* && (ch != '!')*/)
		{
			char bufer[50];
			char c;
			int i;
			bufer[0] = ch;
			for (i = 1;; i++)
			{
				c = fgetc(f);
				if ((c == '\n') || (c == '\t') || (c == ' ') || (c == ';'))
				{
					if (c == '\n')
					{
						line++;
						break;
					}
					else break;
				}
				bufer[i] = c;
			}
			ungetc(c, f);
			bufer[i] = '\0';
			strncpy(res->name, bufer, i);
			res->type = LUnknown;
			res->value = 0;
			res->line = line;
			break;
		}
	}
	return res;
}

void PrintLexemsInFile()
{
	FILE* outTokensFiles;
	char type[MAX_LENGTH_TYPES];
	int i;
	outTokensFiles = fopen("lexems.txt", "w");
	fputs("Table of tokens!\n", outTokensFiles);
	fprintf(outTokensFiles, "=======================================================\n");
	fprintf(outTokensFiles, "%s\t%12s%20s%10s%5s\n", "#", "NAME", "TYPE", "VALUE", "LINE");
	fprintf(outTokensFiles, "=======================================================\n");
	for (i = 0; i < Data.LexNum; ++i)
	{
		switch (Data.LexTable[i].type)
		{
		case LProgram: strncpy(type, "program", 8); break;
		case LProgramName: strncpy(type, "programName", 12); break;
		case LBody: strncpy(type, "body", 5); break;
		case LData: strncpy(type, "data", 5); break;
		case LVarType: strncpy(type, "var type", 9); break;
		case LIdentifier: strncpy(type, "identifier", 11); break;
		case LEnd: strncpy(type, "end", 4); break;
		case LScan: strncpy(type, "read", 5); break;
		case LPrint: strncpy(type, "write", 6); break;
		case LWhile: strncpy(type, "while", 6); break;
		case LDo: strncpy(type, "do", 3); break;
		case LNewValue: strncpy(type, "new value", 10); break;
		case LAdd: strncpy(type, "add", 4); break;
		case LSub: strncpy(type, "sub", 4); break;
		case LMul: strncpy(type, "mul", 4); break;
		case LDiv: strncpy(type, "div", 4); break;
		case LMod: strncpy(type, "mod", 4); break;
		case LEqu: strncpy(type, "equ", 4); break;
		case LNotEqu: strncpy(type, "not equ", 8); break;
		case LGreate: strncpy(type, "greate", 7); break;
		case LLess: strncpy(type, "less", 5); break;
		case LNot: strncpy(type, "not", 4); break;
		case LAnd: strncpy(type, "and", 4); break;
		case LOr: strncpy(type, "or", 3); break;
		case LLBraket: strncpy(type, "left braket", 12); break;
		case LRBraket: strncpy(type, "right braket", 13); break;
		case LNumber: strncpy(type, "number", 7); break;
		case LSeparator: strncpy(type, "separator", 10); break;
		case LComma: strncpy(type, "comma", 6); break;
		case LEOF: strncpy(type, "end of file", 12); break;
		case LUnknown: strncpy(type, "unknown", 8); break;
		}
		fprintf(outTokensFiles, "%d\t%12s%20s%10d%5d\n", i /*+ 1*/, Data.LexTable[i].name, type, Data.LexTable[i].value, Data.LexTable[i].line);
	}
	fclose(outTokensFiles);
}