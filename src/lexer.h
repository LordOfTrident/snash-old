#ifndef __LEXER_H_HEADER_GUARD__
#define __LEXER_H_HEADER_GUARD__

mod_export char *tokstrings[WHILE + 1] = {
    NULL,	/* NULLTOK	  0  */
    ";",	/* SEPER	     */
    "\\n",	/* NEWLIN	     */
    ";",	/* SEMI		     */
    ";;",	/* DSEMI	     */
    "&",	/* AMPER	  5  */
    "(",	/* INPAR	     */
    ")",	/* OUTPAR	     */
    "||",	/* DBAR		     */
    "&&",	/* DAMPER	     */
    ">",	/* OUTANG	  10 */
    ">|",	/* OUTANGBANG	     */
    ">>",	/* DOUTANG	     */
    ">>|",	/* DOUTANGBANG	     */
    "<",	/* INANG	     */
    "<>",	/* INOUTANG	  15 */
    "<<",	/* DINANG	     */
    "<<-",	/* DINANGDASH	     */
    "<&",	/* INANGAMP	     */
    ">&",	/* OUTANGAMP	     */
    "&>",	/* AMPOUTANG	  20 */
    "&>|",	/* OUTANGAMPBANG     */
    ">>&",	/* DOUTANGAMP	     */
    ">>&|",	/* DOUTANGAMPBANG    */
    "<<<",	/* TRINANG	     */
    "|",	/* BAR		  25 */
    "|&",	/* BARAMP	     */
    "()",	/* INOUTPAR	     */
    "((",	/* DINPAR	     */
    "))",	/* DOUTPAR	     */
    "&|",	/* AMPERBANG	  30 */
    ";&",	/* SEMIAMP	     */
    ";|",	/* SEMIBAR	     */
};

extern char *toksList[] = {
	"&&",
	"\\n"
};

#endif // __LEXER_H_HEADER_GUARD__
