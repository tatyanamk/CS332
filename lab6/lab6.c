#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 1024
#define MAX_ITEMS 22555

struct listing {
    int id, host_id, minimum_nights, number_of_reviews, calculated_host_listings_count, availability_365;
    char *host_name, *neighbourhood_group, *neighbourhood, *room_type;
    float latitude, longitude, price;
};

struct listing getfields(char* line){
    struct listing item;

    item.id = atoi(strtok(line, ","));
    item.host_id = atoi(strtok(NULL, ","));
    item.host_name = strdup(strtok(NULL, ","));
    item.neighbourhood_group = strdup(strtok(NULL, ","));
    item.neighbourhood = strdup(strtok(NULL, ","));
    item.latitude = atof(strtok(NULL, ","));
    item.longitude = atof(strtok(NULL, ","));
    item.room_type = strdup(strtok(NULL, ","));
    item.price = atof(strtok(NULL, ","));
    item.minimum_nights = atoi(strtok(NULL, ","));
    item.number_of_reviews = atoi(strtok(NULL, ","));
    item.calculated_host_listings_count = atoi(strtok(NULL, ","));
    item.availability_365 = atoi(strtok(NULL, ","));

    return item;
}

int host_name_comparison(const void *a, const void *b) {
    const struct listing *item1 = (const struct listing *)a;
    const struct listing *item2 = (const struct listing *)b;
    return strcmp(item1->host_name, item2->host_name);
}

int price_comparison(const void *a, const void *b) {
    const struct listing *item1 = (const struct listing *)a;
    const struct listing *item2 = (const struct listing *)b;
    if (item1->price < item2->price) return -1;
    else if (item1->price > item2->price) return 1;
    else return 0;
}

int main(int argc, char* args[]) {
    struct listing list_items[MAX_ITEMS];
    char line[LINESIZE];
    int count = 0;

    FILE *input_file = fopen("listings.csv", "r");
    if (input_file == NULL) {
        printf("There was an reading input file.\n");
        exit(-1);
    }

    if (fgets(line, LINESIZE, input_file) == NULL) {
        printf("There was an reading header.\n");
        exit(-1);
    }


    while (fgets(line, LINESIZE, input_file) != NULL && count < MAX_ITEMS) {
        list_items[count++] = getfields(line);
    }
    fclose(input_file);

    qsort(list_items, count, sizeof(struct listing), host_name_comparison);

 
    FILE *output_file_by_host = fopen("sorted_by_host_name.csv", "w");
    if (output_file_by_host == NULL) {
        printf("There was an error generating an output file sorted by the host name.\n");
        exit(-1);
    }

    fprintf(output_file_by_host, "id,host_id,host_name,neighbourhood_group,neighbourhood,latitude,longitude,room_type,price,minimum_nights,number_of_reviews,calculated_host_listings_count,availability_365\n");

    for (int i = 0; i < count; i++) {
        fprintf(output_file_by_host, "%d,%d,%s,%s,%s,%f,%f,%s,%f,%d,%d,%d,%d\n",
                list_items[i].id, list_items[i].host_id, list_items[i].host_name,
                list_items[i].neighbourhood_group, list_items[i].neighbourhood,
                list_items[i].latitude, list_items[i].longitude, list_items[i].room_type,
                list_items[i].price, list_items[i].minimum_nights, list_items[i].number_of_reviews,
                list_items[i].calculated_host_listings_count, list_items[i].availability_365);
    }
    fclose(output_file_by_host);

    qsort(list_items, count, sizeof(struct listing), price_comparison);


    FILE *output_file_by_price = fopen("sorted_by_price.csv", "w");
    if (output_file_by_price == NULL) {
        printf("There was an error generating an output file for sorted by price\n");
        exit(-1);
    }

    fprintf(output_file_by_price, "id,host_id,host_name,neighbourhood_group,neighbourhood,latitude,longitude,room_type,price,minimum_nights,number_of_reviews,calculated_host_listings_count,availability_365\n");

    for (int i = 0; i < count; i++) {
        fprintf(output_file_by_price, "%d,%d,%s,%s,%s,%f,%f,%s,%f,%d,%d,%d,%d\n",
                list_items[i].id, list_items[i].host_id, list_items[i].host_name,
                list_items[i].neighbourhood_group, list_items[i].neighbourhood,
                list_items[i].latitude, list_items[i].longitude, list_items[i].room_type,
                list_items[i].price, list_items[i].minimum_nights, list_items[i].number_of_reviews,
                list_items[i].calculated_host_listings_count, list_items[i].availability_365);
    }
    fclose(output_file_by_price);

    return 0;
}
