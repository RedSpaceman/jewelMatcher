
class Jewel {

		int x, y;
		char jewelType;
		char getRandomJewelType();
	public:	
		
		Jewel();
		Jewel(int newX, int newY);
		Jewel(int newX, int newY, char newJewelType);
		int getX();
		int getY();
		char getJewelType();
		
		void drawJewel();
		
};


