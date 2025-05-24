package cs250.exercises;
import java.util.ArrayList;

abstract class BankActions {
    protected ArrayList<Account> accounts;
    public abstract void addAccount(Account account);
    public abstract Account findAccount(String name);
    public abstract void listAccounts();
    
    public void performTransactions(String name, double amount, String type) {
        Account account = findAccount(name);
        if (account != null) {
            if (type.equals("deposit"))
                account.deposit(amount);
            else if (type.equals("withdraw"))
                account.withdraw(amount);
        }
    }
}

/*create a public Bank class that extends BankActions; complete the abstract methods in the Bank class*/
public class Bank extends BankActions {
    public Bank() {
        accounts = new ArrayList<>();
    }
    @Override
    public void addAccount(Account account) {
        accounts.add(account);
    }
    @Override
    public Account findAccount(String name) {
        for(Account account : accounts) {
            if(account.getName().equals(name)){
                return account;
            }
        }
        return null;
    }

    @Override
    public void listAccounts() {
        for (Account account : accounts) {
            System.out.println(account);
        }
    }
}//end of class