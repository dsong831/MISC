#ifndef _SLIB_H_
#define	_SLIB_H_

#define	TERMINATE_CHAR	'\r'


void	init_slib(void);
int		getstring(void);
char	*scani(char *s, unsigned int *result);
char	*scans(char *s,char *result);

void	htod(int hvalue,int n);
void	ftod(double fvalue,int n,int e);

extern	char	InData[];
//extern	char	OutData[];
extern	int	InFlag;
//extern	int	OutFlag;
extern	int	InCount,OutCount;

#endif
