
bool is_prime (int n){
	if(!n) return false;
	for(int i = 2; i<= n/2; i++){
		if(!(n % i)) return false;
	}
	return true;
}
