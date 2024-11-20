def f(x):
    return ((x-8)**4) * (x+7)

eps = pow(10,-15)
def step_g(x,r) :
    return (x - r * (x - 8) * (x + 7) / (4 * (x + 7) + (x - 8)))

def calculate(x,m):
    val =  x
    cnt = 0
    old = 100000

    while(abs(val - old) > eps):
        old = val
        if cnt > 10000:
            print("failed with x: ",x)
            break
        
        val = step_g(val,m)

        cnt += 1
    return val,cnt
xs =[2,1,7,3,-8,100,1000]

for x in xs:
    print(x,":",calculate(x,4))
    print(x,":",calculate(x,1))
