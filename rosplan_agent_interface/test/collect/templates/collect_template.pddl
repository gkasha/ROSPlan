(define (problem {{problem}})
(:domain {{domain}})
(:objects
{% for i in instances %}{% for v in instances[i] %}{{v}} {% endfor %}- {{i}}
{% endfor %}
)
(:init

{% for p in predicates %}{% for v in predicates[p] %}
{{v}}{% endfor %}
{% endfor %}


)
(:goal (and
    {% for goal in goals %}
    {{goal}}{% endfor %}
)))
