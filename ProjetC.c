#include <stdio.h>
#include <string.h>
#include <time.h>

#define nombre_voiture 100
#define nombre_Client 100
#define nombre_Location 100

void getCurrentDate(char *date)
{
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    strftime(date, 20, "%m/%d/%Y", tm_info);
}
void calculateEndDate(char *startDate, char *endDate, int duration)
{
    struct tm tm_info;
    memset(&tm_info, 0, sizeof(struct tm));
    sscanf(startDate, "%d/%d/%d", &tm_info.tm_mon, &tm_info.tm_mday, &tm_info.tm_year);
    tm_info.tm_mon -= 1;
    tm_info.tm_year -= 1900;
    // Calculate the end date by adding the duration to the start date
    tm_info.tm_mday += duration;

    // Normalize the date
    mktime(&tm_info);

    strftime(endDate, 20, "%m/%d/%Y", &tm_info);
}

struct Car
{
    int id;
    char etat[20];
    char Brand[50];
    char model[50];
    int year;
    char matricule[20];
    float rentalPrice;
    char status[20];
};
struct Client
{
    int id;
    char first_name[20];
    char last_name[10];
    char email[40];
    char gender[10];
};
struct Location
{
    int location_number;
    int voiture_Id;
    int customer_id;
    char rental_start_date[20];
    char rental_end_date[20];
    int rental_duration;
    float rental_price;
};

FILE *fv;
FILE *fc;
FILE *fl;

struct Car cars[nombre_voiture];
struct Client client[nombre_Client];
struct Location location[nombre_Location];

int numClient = 0;
int numVoiture = 0;
int numLocation = 0;

int loadCARS()
{
    // fichier de voiture

    fv = fopen("voiture.txt", "r");

    if (fv == NULL)
    {
        printf("Erreur d'ouverture !\n");
        return 1;
    }
    // Read data from the file
    while (!feof(fv))
    {
        fscanf(fv, "%d %s %s %s %d %s %f %s",
               &cars[numVoiture].id,
               cars[numVoiture].etat,
               cars[numVoiture].Brand,
               cars[numVoiture].model,
               &cars[numVoiture].year,
               cars[numVoiture].matricule,
               &cars[numVoiture].rentalPrice,
               cars[numVoiture].status);
        numVoiture++;
    }

    fclose(fv);
    return 0;
}
void loadClient()
{
    // fichier de client

    fc = fopen("client.txt", "r");

    if (fc == NULL)
    {
        printf("Erreur d'ouverture !\n");
    }
    // Read data from the file
    while (!feof(fc))
    {
        fscanf(fc, "%d %s %s %s %s ",
               &client[numClient].id,
               client[numClient].first_name,
               client[numClient].last_name,
               client[numClient].email,
               client[numClient].gender);

        numClient++;
    }

    fclose(fc);
}
void loadLOCATION()
{
    // fichier de location

    fl = fopen("location.txt", "r");

    if (fl == NULL)
    {
        printf("Erreur d'ouverture !\n");
    }

    // Read data from the file
    while (

        fscanf(fl, "%d %d %d %s %s %d %f",
               &location[numLocation].location_number,
               &location[numLocation].voiture_Id,
               &location[numLocation].customer_id,
               location[numLocation].rental_start_date,
               location[numLocation].rental_end_date,
               &location[numLocation].rental_duration,
               &location[numLocation].rental_price) == 7)
    {

        numLocation++;
    }
    fclose(fl);
}
void saveData()
{
    FILE *voituref = fopen("voiture.txt", "w");
    if (voituref != NULL)
    {
        for (int i = 0; i < numVoiture; i++)
        {
            fprintf(voituref, "%d %s %s %s %d %s %.3f %s\n", cars[i].id,
                    cars[i].etat,
                    cars[i].Brand,
                    cars[i].model,
                    cars[i].year,
                    cars[i].matricule,
                    cars[i].rentalPrice,
                    cars[i].status);
        }
        fclose(voituref);
    }

    FILE *locationf = fopen("location.txt", "w");
    if (locationf != NULL)
    {
        for (int i = 0; i < numLocation; i++)
        {
            fprintf(locationf, "%d %d %d %s %s %d %.3f\n", i,
                    location[i].voiture_Id,
                    location[i].customer_id,
                    location[i].rental_start_date,
                    location[i].rental_end_date,
                    location[i].rental_duration,
                    location[i].rental_price);
        }
        fclose(locationf);
    }
    FILE *clientf = fopen("client.txt", "w");
    if (clientf != NULL)
    {
        for (int i = 0; i < numClient; i++)
        {
            fprintf(clientf, "%d %s %s %s %s \n",
                    client[i].id,
                    client[i].first_name,
                    client[i].last_name,
                    client[i].email,
                    client[i].gender);
        }
        fclose(clientf);
    }
}
int linklocclient(int id)
{
    int s = -1;
    for (int i = 0; i < numClient; i++)
    {
        if (client[i].id == id)
        {
            s = i;
            break;
        }
    }
    return s;
}
int linklocvoiture(int id)
{
    int s = 0;
    for (int i = 0; i < numVoiture; i++)
    {
        if (cars[i].id == id)
        {
            s = i;
            break;
        }
    }
    return s;
}

// 1--------------------------------
void searchCars()
{
    char searchBrand[50];
    char Model[50];

    printf("Donner la Brand de la voiture que vous cherchez(0 pour ignorer): ");
    scanf("%s", &searchBrand);

    printf("Donner le Model  de la voiture que vous cherchez(0 pour ignorer): ");
    scanf("%s", &Model);

    printf("-------------------------------------------------------------------------------\n");
    printf("| %-3s | %-15s | %-15s | %-15s | %-15s |\n", "ID", "Brand", "Model", "Matricule", "Etat");
    for (int i = 0; i < numVoiture; i++)
    {
        if ((searchBrand[0] == '0' || strcmp(cars[i].Brand, searchBrand) == 0) &&
            (Model[0] == '0' || cars[i].model == Model) &&
            (strcmp(cars[i].etat, "EnService") == 0) && (strcmp(cars[i].status, "Disponible") == 0))
        {

            printf("+-----+-----------------+-----------------+-----------------+-----------------+\n");
            printf("| %-3d | %-15s | %-15s | %-15s | %-15s |\n", cars[i].id, cars[i].Brand, cars[i].model, cars[i].matricule, cars[i].etat);
        }
    }
    printf("-------------------------------------------------------------------------------\n");
}
// 2--------------------------------
void louerVoiture()
{
    char searchBrand[50];
    char Model[50];
    int clientId;
    int duree;

    printf("Donner la Brand de la voiture que vous cherchez(0 pour ignorer): ");
    scanf("%s", &searchBrand);

    printf("Donner le Model  de la voiture que vous cherchez(0 pour ignorer): ");
    scanf("%s", &Model);

    // Recherche de la voiture
    for (int i = 0; i < numVoiture; i++)
    {
        if ((strcmp(cars[i].Brand, searchBrand) == 0 || searchBrand[0] == '0') &&
            (strcmp(cars[i].model, Model) == 0 || Model[0] == '0') &&
            strcmp(cars[i].status, "Disponible") == 0 &&
            strcmp(cars[i].etat, "EnService") == 0)
        {

            if (numLocation < nombre_Location)
            {
                printf("Entrer votre ID Client: ");
                scanf("%d", &clientId);
                // ajouter un contact
                if (linklocclient(clientId) == -1)
                {
                    printf("*******Creation d'un nouveau client*******\n");
                    client[numClient].id = clientId;
                    printf("Entrer votre nom: ");
                    scanf("%s", &client[numClient].first_name);
                    printf("Entrer votre prenom: ");
                    scanf("%s", &client[numClient].last_name);
                    printf("Entrer votre email: ");
                    scanf("%s", &client[numClient].email);
                    printf("Entrer votre genre: ");
                    scanf("%s", &client[numClient].gender);
                    numClient++;
                    printf("*******Client cree avec succee*******\n");
                }

                printf("Entrer la duree de location: ");
                scanf("%d", &duree);

                // Enregistrement de la location
                location[numLocation].location_number = numLocation;
                location[numLocation].voiture_Id = cars[i].id;
                location[numLocation].customer_id = clientId;
                getCurrentDate(location[numLocation].rental_start_date);
                calculateEndDate(location[numLocation].rental_start_date, location[numLocation].rental_end_date, duree);
                location[numLocation].rental_duration = duree;
                location[numLocation].rental_price = (cars[i].rentalPrice) * duree;

                numLocation++;
                strcpy(cars[i].status, "NonDisponible");

                printf("Voiture louee avec succes.\n");
            }
            else
            {
                printf("Impossible de louer plus de %d voitures.\n", nombre_Location);
            }

            return;
        }
    }

    printf("Aucune voiture trouvee.\n");
}
// 3--------------------------------
int carDescription()
{
    int carId;
    printf("Donner l'ID de la voiture dont vous voulez voir la discription : ");
    scanf("%d", &carId);

    // Search for the car in the list
    for (int i = 0; i < numVoiture; i++)
    {
        if (cars[i].id == carId)
        {
            printf("---------------------------------------------------------------------------------------------------\n");
            printf("| 0.ID | 1.Etat     | 2.Brand   | 3.Model   | 4.Annee | 5.Matricule   | 6.Prix/Jour| 7.Status      |\n");
            printf("+------+------------+-----------+-----------+---------+---------------+------------+---------------+\n");
            printf("|  %-3d | %-10s | %-9s | %-9s | %-7d | %-13s | %-10.2f | %-13s |\n",
                   cars[i].id,
                   cars[i].etat,
                   cars[i].Brand,
                   cars[i].model,
                   cars[i].year,
                   cars[i].matricule,
                   cars[i].rentalPrice,
                   cars[i].status);
            printf("---------------------------------------------------------------------------------------------------\n");
        }
    }

    printf("Aucue voiture trouve avec cet ID.\n");
    return 0;
}
// 4--------------------------------
void supprimPanne()
{
    int carId;
    printf("Donner l'ID d'une voiture en panne pour la supprimer: ");
    scanf("%d", &carId);

    // Search for the car in the list
    for (int i = 0; i < numVoiture; i++)
    {
        if (cars[i].id == carId && strcmp(cars[i].etat, "EnPanne") == 0)
        {
            // Remove the car from the list by shifting elements
            for (int j = i; j < numVoiture; j++)
            {
                cars[j] = cars[j + 1];
            }
            numVoiture--;

            printf("Suppressioon avec succee.\n");
            return;
        }
    }

    printf("Aucune voiture EnPanne n'existe avec cet ID.\n");
}
// 5--------------------------------
void ModifDescrip()
{
    int carId;
    int choice;
    printf("Donner l'ID de voiture a modifier: ");
    scanf("%d", &carId);

    // Search for the car in the array
    for (int i = 0; i < numVoiture; i++)
    {
        if (cars[i].id == carId)
        {
            printf("------------------------------------------------------------------------------\n");
            printf("| 1.Etat | 2.Brand | 3.Model | 4.Annee | 5.Matricule| 6.Prix/Jour | 7.Status |\n");
            printf("------------------------------------------------------------------------------\n");
            printf("Choisire l'attribue que vous vouliez changer :");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                do{
                printf("Donner le nouveau etat de la voiture (EnPanne/EnService)  : ");
                scanf("%s", &cars[i].etat);}
                while(cars[i].etat!="EnPanne" && cars[i].etat!="EnService");
                break;
            case 2:
                printf("Donner la nouvelle Brand de la voiture : ");
                scanf("%s", &cars[i].Brand);
                break;
            case 3:
                printf("Donner le nouveau model de la voiture : ");
                scanf("%s", &cars[i].model);
                break;
            case 4:
                printf("Donner la nouvelle annee de fabrication de la voiture : ");
                scanf("%d", &cars[i].year);
                break;
            case 5:
                printf("Donner la nouvelle matricule de la voiture : ");
                scanf("%s", &cars[i].matricule);
                break;
            case 6:
                printf("Donner la nouvelle prix jourdinal de la location de la voiture : ");
                scanf("%f", &cars[i].rentalPrice);
                break;
            case 7:
                do{
                printf("Donner la nouvelle status de la voiture  de la voiture : ");
                scanf("%f", &cars[i].status);}
                while(cars[i].status!="Disponible" && cars[i].status!="NonDisponible");
                break;

            default:
                break;
            }

            printf("Voiture modifier avec succee .\n les nouveaux attribus sont : \n");
            printf("-------------------------------------------------------------------------------------------------------------------\n");
            printf("| %-3s| %-15s | %-15s | %-15s |%-6s| %-15s|%-12s |%-15s |\n", "ID", "Etat ", "Brand", "Model", "Annee", "Matricule", "Prix/jours", "Status");
            printf("+-----+-----------------+-----------------+-----------------+-------+-----------------+----------+----------------+\n");
            printf("|%-3d | %-15s | %-15s | %-15s |%6d| %-15s|%-12.2f |%-15s |\n", cars[i].id, cars[i].etat, cars[i].Brand, cars[i].model, cars[i].year, cars[i].matricule, cars[i].rentalPrice, cars[i].status);
            printf("-------------------------------------------------------------------------------------------------------------------\n");
            return;
        }
    }

    printf("Aucune voiture trouver avec cette ID.\n");
}
// 6--------------------------------
void affichhistorique()
{
    printf("Histoire des Location:\n");

    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    printf("| LID | voitureID  | clientID     | DateDebutLocation    | DateFinLocation      | PeriodedeLocation| PrixTotaldelocation    |\n");
    printf("+-----+------------+--------------+----------------------+----------------------+------------------+------------------------+\n");
    for (int i = 0; i < numLocation; i++)
    {
        printf("| %3d | %10d | %12d | %20s | %20s | %16d | %22.2f |\n",
               i,
               location[i].voiture_Id,
               location[i].customer_id,
               location[i].rental_start_date,
               location[i].rental_end_date,
               location[i].rental_duration,
               location[i].rental_price);
        printf("+-----+------------+--------------+----------------------+----------------------+------------------+------------------------+\n");
    }
}
// 7--------------------------------
void retourvoiture()
{
    int carId;
    printf("Donner l'ID de la voiture que vous voulez rentrer a cause d'une reclamation: ");
    scanf("%d", &carId);

    for (int i = 0; i < numVoiture; i++)
    {
        if (cars[i].id == carId && strcmp(cars[i].status, "NonDisponible") == 0)
        {
            strcpy(cars[i].status, "Disponible");
            printf("Voiture rentree avec succee.\n");

            return;
        }
        else if (cars[i].id == carId && strcmp(cars[i].status, "Disponibles") ==0)
        {
            printf("Voiture deja a la maison de location");
            return;
        }
    }

    printf("voiture non trouvee.\n");
}
// 8--------------------------------
void voiturehistorique()
{
    int found = 0;
    int carId;
    int k, s;
    printf("Donner l'ID du voiture a consulter :  ");
    scanf("%d", &carId);
    printf("+------+-----------+-----------+-------------+----------+------------+-------------------+----------+-----------+\n");
    printf("| ID   | Brand     | Model     | Customer_ID | Nom      | Prenom     | Rental Start Date | Duration | Prix      |\n");
    printf("+------+-----------+-----------+-------------+----------+------------+-------------------+----------+-----------+\n");
    getchar();

    if (numLocation != 0)
    {
        for (int i = 0; i < numLocation; i++)
        {

            if (location[i].voiture_Id == carId)
            {

                // Car has been rented
                found = 1;
                k = linklocvoiture(carId);
                s = linklocclient(location[i].customer_id);
                printf("| %4d | %-9s | %-9s | %4d | %-12s | %-10s | %-17s | %8d | %9.2f |\n",
                       carId,
                       cars[k].Brand,
                       cars[k].model,
                       client[s].id,
                       client[s].first_name,
                       client[s].last_name,
                       location[i].rental_start_date,
                       location[i].rental_duration,
                       location[i].rental_price);
                printf("+------+-----------+-----------+-----------+------------+------------+-------------------+----------+-----------+\n");
            }
        }

        if (!found)
        {
            printf("| Voiture jamais etais en location .\n");
        }
    }
    else
        printf("| Aucune voiture n'est louer pour le moment ");
}


int main()
{
    loadCARS();
    loadLOCATION();
    loadClient();

    int choice;

    do
    {
        printf("\nMenu Principal\n");
        printf("1. Rechercher des voitures disponibles\n");
        printf("2. Louer une voiture\n");
        printf("3. Afficher la description d'une voiture\n");
        printf("4. Supprimer une voiture en panne\n");
        printf("5. Modifier la description et l'etat d'une voiture\n");
        printf("6. Afficher l'historique des locations\n");
        printf("7. Retour d'une voiture en cas de reclamation\n");
        printf("8. Afficher tous les client qui on louee la voiture desirer\n");
        printf("0. Quitter\n");

        printf("Entrez votre choix : ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            searchCars();
            break;
        case 2:
            louerVoiture();
            break;
        case 3:
            carDescription();
            break;
        case 4:
            supprimPanne();
            break;
        case 5:
            ModifDescrip();
            break;
        case 6:
            affichhistorique();
            break;
        case 7:
            retourvoiture();
            break;
        case 8:
            voiturehistorique();
            break;
        case 0:
            break;

        default:
            printf("Choix non valide. Veuillez reessayer.\n");
        }

        saveData();
    } while (choice != 0);
    return 0;
}