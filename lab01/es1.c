#include <stdio.h>
#include <ctype.h>

char *regex(char *src, char *r){
    if (*r == '\0') return src;             //se ho finito r allora vuol dire che il match è stato completato quindi returno src che risale nelle chaimate 
    if (*src == '\0') return NULL;          // fine src senza match
    if (*src == *r || *r == '.') {
        if (regex(src+1, r+1) != NULL) return src;      // proseguo in entrambe le stringhe per vedere se il match continua, se per qualche motivo ritorna NULL allora si è persa la corrispondenza
    }

    if (*r == '/'){             // caso del carattere maisc/minusc
        if (isupper(*(r+1)) && isupper(*src)){              // controllo i caratteri successivi
            if (regex(src+1, r+2) != NULL) return src;          // stessa logica di prima 
        }

        else if (islower(*(r+1)) && islower(*src)){             
            if (regex(src+1, r+2) != NULL) return src;
        }
    }

    if (*(r) == '['){                   // caso della parentesi
        if (*(r+1) == '^'){          // lettere negate
            int k = 2;                  // contatore per spostarmi nella r
            while (*(r+k) != ']'){              
                if (*src == *(r+k)) return NULL;            // se l'src corrente si trova nelle parentesi, ritorno NULL xk il match non è già più vero
                k++;                
            }
            if (regex(src+1, r+1+k) != NULL) return src;            // se non ha returnato NULL, la lettera di src è consentita e posso andare avanti (balzando le quadre)
        }

        else {                          // lettere consentite, logica simile a sopra
            int k = 1, flag = 0;
            while (*(r+k) != ']'){
                if (*(r+k) == *(src)) flag = 1;             // la lettera va bene, il ciclo continua cmq per capire quanto è lunga la parentesi
                k++;
            }
            if (flag == 1){
                if (regex(src+1, r+k+1) != NULL) return src;            // posso continuare a verificare il match
            }
        }
    }
    return NULL;               // IL MATCH SI è INTERROTTO 
}

int main(){
    char source[] = "vadoinmoto";             // stringa in cui ricerco
    char r[] = ".ado/an[maco]o[^ac].";              // regex da trovare 
    int k = 0, flag = 0;

    while (*(source+k) != '\0' && flag == 0){           // while che continua fino a quando non ho finito la source o quando non ho trovato la prima regex
        char *first_occ = regex(source+k, r);               // chiamo la funzione ricorsiva sul punto della stringa corrente e sempre sull'inizio della regex

        if (first_occ != NULL){                         // se la ricorsione non ha tornato NULL allora ho trovato una regex
            printf("La prima occorrenza della regexp è %s\n", first_occ);
            flag =1;                // interrompo il ciclo
        }
        k++;            // vado aventi nella source
    }

    if (flag == 0) printf ("Nessuna occorenza");        // allora ho finito la stringa senza trovare ricorrenze
    return 0;
}