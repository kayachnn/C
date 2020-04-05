int maxProfit(int* prices, int pricesSize){

    
	int sum = 0;

    int i;
	int j = 0;

	for (int i = 1; i < pricesSize; i++)
	{
		if (prices[i - 1] > prices[i]) {
			j = i;
		}


		if (prices[i - 1] <= prices[i] &&
			(i + 1 == pricesSize || prices[i] > prices[i + 1]))
		{
			sum += (prices[i] - prices[j]);
		}
	}

	return sum;
}
