
package cs250.helpers;
import java.util.Arrays;
public class ExerciseHelper{


public void transposeMatrix(int[][] array) {

    int row = array.length;
    int col = array[0].length; // correct sizing
    int[][] transposed = new int[col][row];


    for (int i =0; i < row; i++) {
        for (int j = 0; j < col; j++) {
             transposed[j][i] = array[i][j];
             // can't print in the loop
             }
    }

    //lets try copying it back into the original array..omg no thats stupid bc its out of bounds << dont do this again!
    for (int i = 0; i < transposed.length; i++){
        array[i] = transposed[i];
    }
    
        }
    }





