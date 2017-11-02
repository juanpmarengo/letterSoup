#include "../main/letterSoup.h"
#include <stdio.h>
#include <assert.h>

int main () {
    Universo u0 = {
    	0, NULL
    };

    Universo u1 = {
    	20, (Palabra[]) {
    		{"ALTAR", NULL},
    		{"AMOR", NULL},
    		{"CALVARIO", NULL},
    		{"CHUPETE", NULL},
    		{"DISCAL", NULL},
    		{"ELECTROENCEFALOGRAMA", NULL},
    		{"EMBALSE", NULL},
    		{"GOLOSA", NULL},
    		{"MACABRA", NULL},
    		{"MONTO", NULL},
    		{"OCEANO", NULL},
    		{"RADIOGRAFIAR", NULL},
    		{"SELVA", NULL},
    		{"TAMBORIL", NULL},
    		{"TAPIADA", NULL},
    		{"TAREA", NULL},
    		{"TOCINO", NULL},
    		{"TUTOR", NULL},
    		{"USADO", NULL},
    		{"VANDALISMO", NULL}
    	}
    };

    SopaDeLetras s0 = {
    	0, 0, NULL
    };

    SopaDeLetras s1 = {
    	20, 20, (char*[]) {
    		"GBAEETQUIYSREMNXARTE",
    		"KBOSAOFLICOOEELRANLP",
    		"JLEAOBTAUTBMRYEDEEQG",
    		"UEAAOLNSUBBOORIDCSUR",
    		"TOCINOOTLAAOCOOTXCCF",
    		"ZWJKSMYGLLOAGERFOHAI",
    		"AHUATAOSTJHRTOAXVULC",
    		"CDPENPEANLAOEAYNMPVM",
    		"UFREVRRXXFJNINMOOEAN",
    		"SMOOMADQITCPEXABQTRX",
    		"ATIYOVNAOEXDQGLSOEIY",
    		"DUTSNERDFXTEEECOGRON",
    		"OYAHTKAAAATUNMOUPBIN",
    		"HAUOOHLEPLEWAAGPCKPL",
    		"UAUALORIKGICYECGNDFL",
    		"HSRYGAAAYNASLBUSXSMN",
    		"ENRRTDZVUBOLMTEHAKEK",
    		"VAAOABQXRAVWXOOQQELU",
    		"AMVHRKNAZBVJLCLACSID",
    		"ABDUMLSELVAAOGYKNANZ"
    	}
    };

    // Buscando una letra en una sopa vacia
    assert(esDireccion(s0, "C", 0, 0, 0) == 0);
    // Buscando una palabra en una sopa vacia
    assert(esDireccion(s0, "CASTOR", 0, 0, 0) == 0);
    // Buscando una palabra en una sopa vacia fuera de su tamaño
    assert(esDireccion(s0, "PERRO", 2, 4, 3) == 0);
    // Buscando una palabra en una sopa que no se encuentra
    assert(esDireccion(s1, "SACO", 2, 4, 5) == 0);
    // Buscando una palabra en una sopa en la cordenada incorrecta
    assert(esDireccion(s1, "TOCINO", 2, 8, 2) == 0);
    // Buscando una palabra en una sopa en la direccion incorrecta
    assert(esDireccion(s1, "TOCINO", 0, 4, 3) == 0);
    // Buscando una palabra fuera de la sopa
    assert(esDireccion(s1, "TOCINO", 21, 21, 3) == 0);
    // Buscando una palabra en una sopa en la coordenada y direccion correcta
    assert(esDireccion(s1, "TOCINO", 0, 4, 2) == 1);
    // Buscando en una direccion inexistente
    assert(esDireccion(s1, "TOCINO", 0, 4, 8) == 0);

    // Buscando una letra en una sopa vacia
    assert(encontrarDireccion(s0, "C", 0, 0) == -1);
    // Buscando una palabra en una sopa vacia
    assert(encontrarDireccion(s0, "CASTOR", 0, 0) == -1);
    // Buscando una palabra en una sopa vacia fuera de su tamaño
    assert(encontrarDireccion(s0, "PERRO", 2, 4) == -1);
    // Buscando una palabra en una sopa que no se encuentra
    assert(encontrarDireccion(s1, "SACO", 2, 4) == -1);
    // Buscando una palabra en una sopa en la cordenada incorrecta
    assert(encontrarDireccion(s1, "TOCINO", 2, 8) == -1);
    // Buscando una palabra fuera de la sopa
    assert(encontrarDireccion(s1, "TOCINO", 21, 21) == -1);
    // Buscando una palabra en una sopa en la coordenada correcta
    assert(encontrarDireccion(s1, "TOCINO", 0, 4) == 2);

    // Buscando una letra en una sopa vacia
    assert(encontrarSolucion(s0, "C") == NULL);
    // Buscando una letra en una sopa vacia
    assert(encontrarSolucion(s0, "CASTOR") == NULL);
    // Buscando una palabra en una sopa que no se encuentra
    assert(encontrarSolucion(s1, "SACO") == NULL);
    // Buscando una palabra en una sopa
    Solucion *solucion = encontrarSolucion(s1, "TOCINO");
    assert(solucion != NULL);
    assert(solucion->x == 0);
    assert(solucion->y == 4);
    assert(solucion->direccion == 2);

    printf("All tests passed!\n");

    return 0;
}
