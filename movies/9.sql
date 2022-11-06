SELECT people.name
FROM movies
JOIN stars  ON movies.id = stars.movie_id
JOIN people ON people.id = stars.person_id
WHERE year = 2004
GROUP BY people.name, people.id
ORDER BY birth;
