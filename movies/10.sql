SELECT people.name
FROM movies, people, directors, ratings
WHERE directors.movie_id = movies.id
AND directors.person_id = people.id
AND ratings.movie_id  = movies.id
AND rating >= 9.0
GROUP BY people.name, people.id;