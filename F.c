#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>  

    char title[100];
    char price[20];
    char shipping[20];
    char description[500];
};

// Function to perform web scraping
void scrape_ebay(const char* item_name);

// Function to get the number of pages from HTML
int get_pages(const char* html);

// Function to get item information from HTML
void get_info(const char* html, struct Item* items, int* item_count);

int main() {
    scrape_ebay("resident evil 7");
    return 0;
}

void scrape_ebay(const char* item_name) {
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        char url[200];
        sprintf(url, "ht", item_name);

        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else {
            
            char* html;
            long length;
            curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &length);
            html = (char*)malloc(length + 1);
            curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &length);
            html = (char*)malloc(length + 1);

            // Check for memory allocation failure
            if (!html) {
                fprintf(stderr, "Memory allocation failed\n");
                return;
            }

            // Null-terminate the string
            html[length] = '\0';

            // Perform the request again to get the HTML
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, (void*)memcpy);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, html);
            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            else {
                
                int pages = get_pages(html);

             
                struct Item items[200];  ge

               
                for (int current_page = 1; current_page <= pages; ++current_page) {
                    
                    char page_url[250];
                    sprintf(page_url, "%s&_pgn=%d", url, current_page);

                   
                    curl_easy_setopt(curl, CURLOPT_URL, page_url);
                    res = curl_easy_perform(curl);

                    if (res != CURLE_OK) {
                        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                        continue;
                    }

                   
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, (void*)memcpy);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, html);
                    res = curl_easy_perform(curl);

                    if (res != CURLE_OK) {
                        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                        continue;
                    }

                    int item_count = 0;
                    get_info(html, items, &item_count);

                    
                    for (int i = 0; i < item_count; ++i) {
                        printf("Title: %s\nPrice: %s\nShipping: %s\nDescription: %s\n\n",
                               items[i].title, items[i].price, items[i].shipping, items[i].description);
                    }
                }
            }

          
            free(html);
        }

       
        curl_easy_cleanup(curl);
    }

    // Cleanup global state
    curl_global_cleanup();
}



void get_info(const char* html, struct Item* items, int* item_count) {
  
        sprintf(items[i].title, "Item %d Title", i + 1);
        sprintf(items[i].price, "$%.2f", 19.99 + i * 5.0);
        sprintf(items[i].shipping, "$%.2f", 5.99);
        sprintf(items[i].description, "Item %d Description", i + 1);
    }

    *item_count = 3;
}
