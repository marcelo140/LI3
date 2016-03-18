
#define BUFF_SIZE 128

int masterTest () {

    FILE * validSales = fopen("Vendas_1MValidas.txt", "r");
    FILE * unvalidSales = fopen("Vendas_1MInvalidas.txt", "r");
    char buf[BUFF_SIZE], *token;

    int lnOk=1, i=0, precoZero = 0;

    while (fgets(buf, BUFF_SIZE, validSales)) {

        token = strtok(line, " \n\r");

        for (i = 0; lnOk && token != NULL; i++){
            switch(i) {
                case 0: lnOk = checkProduct(token) && (lookUp(productCat, token));
                            break;
                case 1: lnOk = ((price = atof(token)) >= 0 && price <= 999.99);
                            break;
                case 2: lnOk = ((quant = atoi(token)) >= 1 && quant <= 200);
                            break;
                case 3: lnOk = !strcmp(token, "P") || !strcmp(token, "N");
                            break;
                case 4: lnOk = checkClient(token) && (lookUp(clientCat, token));
                            break;
                case 5: lnOk = (month = atoi(token) >= 1 && month < 12);
                            break;
                case 6: lnOk = ((filial = atoi(token)) >= 1 && filial <= 3);
                            break;
                default: lnOk = 0;
            }
            token = strtok(NULL, " \n\r");
        }
    }

}
