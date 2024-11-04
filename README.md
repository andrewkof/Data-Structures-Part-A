# Data Structures Phase 1

## Project Description

This project is a basic implementation of a streaming service. It manages movie records across different categories, tracks user activity, and makes movie recommendations. The primary 
focus is on the efficient use of data structures, including linked lists, stacks, and arrays.

Movies are categorized into six genres:
- Horror
- Science-Fiction
- Drama
- Romance
- Documentary
- Comedy
Each user and movie has a unique identifier and is associated with relevant structures that allow for efficient data manipulation and retrieval.

## Features

-**User Registration & Management:** Add and remove users from the service.
-**Movie Management:** Add movies to a catalog, categorize them, and manage their availability.
-**Viewing History:** Maintain a stack-based watch history for each user.
-**Movie Suggestions:** Generate recommendations based on other users’ watch histories.
-**Filtered Searches:** Allow users to search for movies by category and release year.
-**Data Output:** Print summaries of movies and user information.

## Data Structures
**Category List (Array):** Array of six linked lists, each representing a genre. Movies in each genre are stored in sorted order based on their unique identifier (mid).


