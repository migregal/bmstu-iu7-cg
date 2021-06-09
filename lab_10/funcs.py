from math import sin, cos, sqrt

funcs = [
    lambda x, z: sin(x) * sin(z),
    lambda x, z: sin(cos(x)) * sin(z),
    lambda x, z: cos(sqrt(x**2 + z**2)),
    lambda x, z: sqrt(x**2 + z**2),
    lambda x, z: cos(x) * cos(z),
    lambda x, z: sin(z) * cos(x),
    lambda x, z: cos(sin(x * z))
]

funcs_names = [
    "sin(x) * sin(z)",
    "sin(cos(x)) * sin(z)",
    "cos(sqrt(x^2 + z^2))",
    "sqrt(x^2 + z^2)",
    "cos(x) * cos(z)",
    "sin(z) * cos(x)",
    "cos(sin(x * z))"
    ]
