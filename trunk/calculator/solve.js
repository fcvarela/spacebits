//returns true if a variable is defined
function defined(v){
    return typeof(v) != 'undefined';
}


//solves the function f(x) = v using bisection
//x0 and x1 are the initial guesses
//v is zero by default
//d is the precision (result is guaranteed to bee within d from the actual solution, defaults to a maximum of 20 iterations)
//returns Number.NaN if a solution can't be found (bad guesses)
function solve(f, x0, x1, v, d){

    if( !defined(v) ){
        v = 0.0;
    }

    if( !defined(d) ){
        // 10 recursions by default
        d = Math.abs(x1-x0)/(Math.pow(2,20));
    }

    var v0 = f(x0)-v;
    var v1 = f(x1)-v;
    var x2 = (x0+x1)/2.0;
    var v2 = f(x2)-v;

    if( Math.abs(v0-v1) > d){
        if( (v0<0 && v1<0) || (v0>0 && v1>0) ){
            //signal error
            return Number.NaN;
        }
        if( (v0<0 && v2>0) || (v0>0 && v2<0)){
            return solve(f, x0, x2, v, d);
        }else if( (v1<0 && v2>0) || (v1>0 && v2<0)){
            return solve(f, x2, x1, v, d);
        }
    }else{
        return x2;
    }
}
