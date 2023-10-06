#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

// Function to write data to the standard output.
// The fflush function is used to stream the output to the console in real-time.
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	// ptr is a pointer to the data to be written
	const char *data = ptr;

	// start is a pointer to the start of the "content" field in the JSON data
	const char *start = strstr(data, "\"content\":\"");

	// If start is not NULL, then the "content" field was found
	if (start)
	{
		// start is now pointing to the first character after the "content" field
		start += 11;

		// i is the index of the current character in the "content" field
		int i;

		// Loop through each character in the "content" field
		for (i = 0; start[i] != '"'; i++)
		{
			// Print the current character to the console
			putchar(start[i]);
		}

		// Flush the output buffer to ensure that the data is written to the console immediately
		fflush(stdout);
	}

	// Return the number of bytes written to the console
	return size * nmemb;
}

int main()
{
	// Initialize the cURL library
	CURL *curl = curl_easy_init();

	// Set the URL to send the request to
	curl_easy_setopt(curl, CURLOPT_URL, "https://openrouter.ai/api/v1/chat/completions");

	// Set the headers for the request
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "HTTP-Referer: https://...");	// Replace with referer url
	headers = curl_slist_append(headers, "X-Title: ...");	// Replace with app name
	headers = curl_slist_append(headers, "Authorization: Bearer sk-...");	// Replace with API key from OpenRouter
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	// Use a char array instead of a message struct
	char content[] = "Tell me what the internet is";

	// Create a char array to hold the post data
	char post_data[200];

	// Set the role and content for the post data
	char role[] = "user";
	sprintf(post_data, "{\"messages\":[{\"role\":\"%s\",\"content\":\"%s\"}], \"max_tokens\":2000, \"stream\":true}", role, content);

	// Print the post data to the console
	printf("%s\n", post_data);

	// Set the post data for the request
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

	// Set the write function for the response
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

	// Perform the request
	curl_easy_perform(curl);

	// Clean up the cURL handle
	curl_easy_cleanup(curl);

	return 0;
}
