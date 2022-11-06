SELECT title
FROM movies, people, stars, ratings
WHERE movies.id = stars.movie_id
AND stars.person_id = people.id
AND ratings.movie_id  = movies.id
AND name = "Chadwick Boseman"
ORDER BY rating DESC
limit 5;