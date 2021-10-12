#include <stdio.h>

void clear_stdin()
{
}

void get_unum()
{
    // fflush();
    // scanf();
    // clear_stdin();
}

void prog_timeout()
{
}

void enable_timeout_cons()
{
}

int auth(char *login, unsigned int serial)
{
    int len;
    int hash;
    int i;

    login[strcspn(login, "\n")] = 0;
    len = strnlen(login, 32);
    if (len > 5)
    {
        if (ptrace(0, 0, 1, 0) == 0xffffffff)
        {
            puts("[32m.---------------------------.");
            puts("[31m| !! TAMPERING DETECTED !!  |");
            puts("[32m'---------------------------'");
            return (1);
        }
        hash = (login[3] ^ 4919) + 6221293;
        i = 0;
        while (i < len)
        {
            if (login[i] <= 31)
                return (1);
            //    0x0804882f <+231>:   mov    %eax,%ecx			=>  eax = ecx =  login[i]
            //    0x08048831 <+233>:   xor    -0x10(%ebp),%ecx	=> ecx = eax ^ hash((login[3] ^ 4919) + 6221293)
            int A = login[i] ^ hash;
            //    0x08048834 <+236>:   mov    $0x88233b2b,%edx	=> edx = 0x88233b2b
            //    0x08048839 <+241>:   mov    %ecx,%eax			=> eax = A
            //    0x0804883b <+243>:   mul    %edx				=> eax = A * edx 	=> A * 0x88233b2b bitwse 32
            //    0x0804883d <+245>:   mov    %ecx,%eax			=> ecx = eax
            int B = A * 0x88233b2b >> 32;
            //    0x0804883f <+247>:   sub    %edx,%eax			=> eax = eax - edx 	=> A - B
            //    0x08048841 <+249>:   shr    %eax  			=> eax = eax /2		=> (A - B) /2
            //    0x08048843 <+251>:   add    %edx,%eax			=> eax = eax + edx	=> ((A - B) /2) + B
            //    0x08048845 <+253>:   shr    $0xa,%eax			=> eax = eax / 1024 => (((A - B) /2) + B) / 1024
            //    0x08048848 <+256>:   imul   $0x539,%eax,%eax	=> eax = eax * 1337 => ((((A - B) /2) + B) / 1024) * 1337
            int C = ((((A - B) >> 1) + B) >> 10) * 1337;
            //    0x0804884e <+262>:   mov    %ecx,%edx			=> edx = ecx => A
            //    0x08048850 <+264>:   sub    %eax,%edx			=> edx = edx - eax +> A - C
            int D = A - C;
            //    0x08048852 <+266>:   mov    %edx,%eax			=> eax = edx
            hash += D;
            i++;
        }
    }
    if (hash == serial)
        return (0);
    return (1);
}

int main(int ac, char **av)
{
    char login[32];
    unsigned int serial;

    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");
    printf("-> Enter Login: ");
    fgets(login, 32, stdin);
    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");
    printf("-> Enter Serial:");
    scanf("%u", &serial);
    if (auth(login, serial) == 0)
    {
        puts("Authenticated!");
        system("/bin/sh");
        return (0);
    }
    return (1);
}
