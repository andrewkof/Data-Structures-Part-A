# Data Structures Phase 1

## General Description
In this project, Ι implement a simplified movie streaming service. The service offers movies organized into different thematic categories. Users can register on the platform, watch movies 
and add them to their watch history, receive movie recommendations based on the watch history of other users, and perform filtered searches across movie categories.

## Detailed Description of the Implementation

The service you will implement organizes its available movies into six thematic categories: Horror, Science Fiction, Drama, Romance, Documentary, and Comedy. Each movie belongs to only one 
category and has a unique identifier. The categorization of movies will be implemented using a six-position array, called the category array. Each position in the array contains a pointer 
(of type ```struct movie *```) pointing to the first element of the linked list of movies for that particular category. This list is a singly linked list, sorted in ascending order based on the 
movie identifier (```mid```). A node in this list represents a movie within the category, defined by a structure (```struct movie```) with the following fields:

- **info:** A helper structure of type ```struct movie_info``` that describes the available information for a movie. Its fields are as follows:
   - something




