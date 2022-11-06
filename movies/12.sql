SELECT title
FROM movies, people, stars, ratings
WHERE movies.id = stars.movie_id
AND stars.person_id = people.id
AND ratings.movie_id  = movies.id
AND name = "Helena Bonham Carter"
AND title IN (SELECT title
FROM movies, people, stars, ratings
WHERE movies.id = stars.movie_id
AND stars.person_id = people.id
AND ratings.movie_id  = movies.id
AND name = "Johnny Depp");