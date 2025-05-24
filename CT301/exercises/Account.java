package cs250.exercises;
import java.util.ArrayList;

abstract class AccountActions {
    protected String name;
    protected double balance;
    protected AccountActions(String name, double balance) {
        this.name = name;
        this.balance = balance;
    }
    protected abstract String getName();
    protected abstract double getBalance();
    // make sure to check if the deposit is > 0
    protected abstract void deposit(double amount);
    // make sure to check if  0 < amount <= balance
    protected abstract void withdraw(double amount);
    
    public String toString() {
        return getName() + " : $" + String.format("%.2f",getBalance());
    }
}



/* create a public Account class that extends AccountActions; complete the abstract methods in the Account class*/


public class Account extends AccountActions {
    public Account(String name, double balance) {
        super(name, balance);
    }

    @Override
    protected String getName() {
        return name;
    }
    @Override
    protected double getBalance() {
        return balance;
    }
    @Override
    protected void deposit(double amount){
        if (amount > 0) {
            balance += amount;
        }
    }
    @Override
    protected void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
        }
    }
}// end of class