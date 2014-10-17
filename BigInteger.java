import java.io.*;
import java.util.*;
import java.math.BigInteger;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.InputStream;   

class Main{
	public static void main(String[] args){
		try{
			//System.setIn(new BufferedInputStream(new FileInputStream("")));
			//System.setOut(new PrintStream(new File(""));
			System.setIn( new FileInputStream("in.txt")  );
			System.setOut( new PrintStream(new FileOutputStream("out.txt") ) );
		}catch(FileNotFoundException e){
			e.printStackTrace();
		}
		Scanner cin = new Scanner( System.in );
		int t = cin.nextInt();
		BigInteger []a = new BigInteger[2001];
		for(int tt=1; tt<=t; tt++){
			int n = cin.nextInt();
			BigInteger result = new BigInteger("0");
			for(int i=0; i<n; i++){
				a[i] = cin.nextBigInteger();
				for(int j=0; j<i; j++){
					int v = a[i].compareTo(a[j]);
					if( v==-1 ) result = result.gcd( a[j].subtract(a[i]) );
					else if( v==1 ) result = result.gcd(a[i].subtract(a[j]) );
				}
			}
			System.out.print( "Case #"+tt+": " );
			BigInteger ans = a[0].remainder( result );
			if( ans.compareTo(new BigInteger("0"))==0 )
				System.out.println( "0" );
			else
				System.out.println( result.subtract(ans).toString() );
		}
	}
}