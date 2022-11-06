SELECT people.name
    FROM movies, people, stars
    WHERE movies.id = stars.movie_id
    AND stars.person_id = people.id
    AND name <> "Kevin Bacon"
    AND movies.id IN (  SELECT distinct movies.id
                        FROM movies, people, stars
                        WHERE movies.id = stars.movie_id
                        AND stars.person_id = people.id
                        AND name = "Kevin Bacon"
                        AND birth = 1958 )
GROUP BY people.id, people.name
;
