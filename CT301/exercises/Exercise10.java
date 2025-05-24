package cs250.exercises;
import java.util.ArrayList;

public class Exercise10 {
    public static void main (String[] args) {
        Bank bank = new Bank();

        Account acc1 = new Account("Alice",1000);
        Account acc2 = new Account("Bob", 500);

        bank.addAccount(acc1);
        bank.addAccount(acc2);
        bank.listAccounts();

        bank.performTransactions("Alice", 200, "deposit");
        bank.performTransactions("Bob", 200, "withdraw");

        bank.listAccounts();
    }//end of main

}// end of class