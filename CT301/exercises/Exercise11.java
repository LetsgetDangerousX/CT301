package cs250.exercises;
import java.util.Arrays;
import cs250.helpers.ExerciseHelper;
import matrices.IntMatrix;

public class Exercise11 {

    public static void main(String[] args){
        // now that the files are communicating we can make some print statements! yay

        // Create first random matrix
        IntMatrix intMatrix = new IntMatrix();
        int[][] matrix = intMatrix.generateIntMatrix();
        System.out.println(Arrays.deepToString(matrix));

        // transpose matrix
        ExerciseHelper helper = new ExerciseHelper();
        
        helper.transposeMatrix(matrix);
        System.out.println(Arrays.deepToString(matrix));

      
       
    }
}
