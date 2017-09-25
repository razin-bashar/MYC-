
public class com_revesoft_itelmobiledialer_e2eencryption_rsa {
	 
	   public native String getEncryptedString(String text, String Key, String Modular);
	   public native String getDecryptedString(String text, String Key, String Modular);
	   public native byte[] getEncryptedByte(String text, String Key, String Modular);
	   public native String getDecryptedByte(byte[] text, String Key, String Modular);
       public native String getCRTDecryptedString(String text, String Key, String Prime_p, String Prime_q);
	   public native String getCRTDecryptedByte(byte[] text, String Key, String Prime_p, String Prime_q);
	
	   public static void main(String[] args)
	   {
		   
		 String p="18532395500947174450709383384936679868383424444311405679463280782405796233163977";
		 String q="39688644836832882526173831577536117815818454437810437210221644553381995813014959";
		 String privateKey = "485854913264130973250569975301779986053271807089053901130886614918715325181887127508721366829749313766022681379491380069517013552827610478044995541453411938599";
		 String publicKey = "44822481511601066098713481453161748979849764719554039096395688045048053310178487";
		 String modular = "735525663012812016752819539011833753097161922338996251151142357377434822209653816935250412578067283255807863837347025544141542626774352903091100262280200931943";
	     System.loadLibrary("RSA");
	     com_revesoft_itelmobiledialer_e2eencryption_rsa sample = new com_revesoft_itelmobiledialer_e2eencryption_rsa();
						 
		 String G = sample.getEncryptedString("the bangladesh djhjfhjshdjfhdvhjbvhbsdhjthe bangladesh",publicKey,modular);
		 String GD = sample.getCRTDecryptedString(G,privateKey,p,q);
		 
		 System.out.println("Encryptedmsg: " + G);
		 System.out.println("Decryptedmsg: " + GD);
		 
		 
						 
		 byte[] G_b = sample.getEncryptedByte("hi mir razin bashar",publicKey,modular);
		 byte[] buff = G_b;
		 for(byte c : buff) {
			System.out.format("[%d] ", c);
		 }
		 System.out.println();
		 try{
			 
			System.out.println("Encryptedmsg: " + new String(G_b, 0, G_b.length, "ASCII"));
			
		 }catch(Exception e){
			 System.out.println("error ");
		 }
		 
		 String GD_b = sample.getCRTDecryptedByte(G_b,privateKey,p,q);
		 
		 
		 System.out.println("Decryptedmsg: " + GD_b);
	   }
}
