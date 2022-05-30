using System;
using System.Linq;

namespace ConsoleApp4
{
    class Program
    {

        static void Main(string[] args)
        {
            Program program = new Program();


            // Test Cases 1 : 
            int[] array = new int[] { 1, 8, 12, 21, 3 };
            int[] indeices = program.twoSumTarget(array, 15);
            if (indeices != null)
                Console.WriteLine("{0},{1}", indeices[0], indeices[1]);

            // Test Case 2 :  
            //Console.WriteLine("***********************************************"); 
            int[,] testCase1 = { { 1, 2 }, { 3, 4 } };
            testCase1 = program.RotateTimes(1, testCase1);
            program.DisplayMatrix(testCase1);
            //int[,] testCase2 = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
        }

        /// <summary>
        /// Rotate provided matrix provided time.
        /// </summary>
        /// <param name="numOfTimes">Number of time to rotate</param>
        /// <param name="unRotatedMatrix">Souce matrix</param>
        /// <returns>90 degree rotated matrix * times</returns>
        public int[,] RotateTimes(int numOfTimes, int[,] unRotatedMatrix)
        {
            for (int i = 0; i < numOfTimes; i++)
            {
                unRotatedMatrix = rotateArrayAntiClockwise(unRotatedMatrix);
            }
            return unRotatedMatrix;
        }
        /// <summary>
        /// This will rotate provided matrix to 90 degrees.
        /// </summary>
        /// <param name="unRotatedMatrix">unrotated source matrix</param>
        /// <returns> Rotated Source Matrix</returns>
        private int[,] rotateArrayAntiClockwise(int[,] unRotatedMatrix)
        {
            int width;
            int height;
            int[,] dst;

            width = unRotatedMatrix.GetUpperBound(0) + 1;
            height = unRotatedMatrix.GetUpperBound(1) + 1;

            if (width != height) return unRotatedMatrix;
            dst = new int[height, width];

            for (int row = 0; row < height; row++)
            {
                for (int col = 0; col < width; col++)
                {
                    int newRow;
                    int newCol;

                    newRow = col;
                    newCol = height - (row + 1);
                    dst[newCol, newRow] = unRotatedMatrix[col, row];
                }
            }
            return dst;
        }
        /// <summary>
        /// To print the matrix values in to Console.
        /// </summary>
        /// <param name="mat">Matrix to print</param>
        void DisplayMatrix(int[,] mat)
        {
            for (int i = 0; i < mat.GetLength(0); i++)
            {
                for (int j = 0; j < mat.GetLength(1); j++)
                    Console.Write(" " + mat[i, j]);
                Console.WriteLine();
            }
            Console.WriteLine();
        }
        /// <summary>
        /// This method find sorted indices from given array, which sum values are equal to target.
        /// </summary>
        /// <param name="nums">unsorted array</param>
        /// <param name="target">Target Array</param>
        /// <returns>Sorted indices which sum is equal to target</returns>
        public int[] twoSumTarget(int[] nums, int target)
        {
            var listObject = nums.ToList();
            //Validations
            if (listObject == null || listObject.Count() < 2)
                return null;

            //Logic 
            for (int i = 0; i < listObject.Count; i++)
            {
                int firstNumber = listObject[i];
                int secondNumber = target - firstNumber;
                int index = listObject.IndexOf(secondNumber);
                if (index != -1) // Condition says there will be only one pair match
                {
                    var foundIndices = new int[] { i, index };
                    Array.Sort(foundIndices);
                    return foundIndices;
                }
            }
            return null;
        }
    }
}

