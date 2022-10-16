import java.io.*;
import java.util.Arrays;

class BankersUtil{
    int no_of_process;
    int no_of_resources;
    int safe_sequence[];
    int available_resources[];

    int allocated_resources[][];
    int max_resources_request[][];
    int need_resources[][];

    
    BankersUtil(int no_of_process, int no_of_resources){
        this.no_of_process = no_of_process;
        this.no_of_resources = no_of_resources;
        this.safe_sequence = new int[no_of_process];
        this.available_resources = new int[no_of_resources];
        this.allocated_resources = new int[no_of_process][no_of_resources];
        this.max_resources_request = new int[no_of_process][no_of_resources];
        this.need_resources = new int[no_of_process][no_of_resources];
    }

    public void setNoOfInstanceForResource(int resource_no, int no_of_instances){
        this.available_resources[resource_no] = no_of_instances;
    }

    public void setMaxResourceForProcess(int process_no, int resource_no, int no_of_instances){
        this.max_resources_request[process_no][resource_no] = no_of_instances;
    }


    public void calculateNeedMatrix(){
        for(int i=0;i<no_of_process;i++){
            for(int j=0;j<no_of_resources;j++){
                need_resources[i][j] = max_resources_request[i][j] - allocated_resources[i][j];
            }
        }
    }

    public void displayData(){
        System.out.println("Process No\tAllocated\tMax\t\tNeed\t\tAvailable");
        System.out.println("----------\t---------\t--------\t--------\t---------");
        for(int i=0;i<no_of_process;i++){
            System.out.print("P"+i+"\t\t");
            for(int j=0;j<no_of_resources;j++){
                System.out.print(allocated_resources[i][j]+" ");
            }
            System.out.print("\t");
            for(int j=0;j<no_of_resources;j++){
                System.out.print(max_resources_request[i][j]+" ");
            }
            System.out.print("\t");
            for(int j=0;j<no_of_resources;j++){
                System.out.print(need_resources[i][j]+" ");
            }
            System.out.print("\t");
            if(i==0){
                for(int j=0;j<no_of_resources;j++){
                    System.out.print(available_resources[j]+" ");
                }
            }
            System.out.println();
        }
        System.out.println();
    }

    public void displaySafeSequence(){
        System.out.print("Safe Sequence: ");
        for(int i=0;i<no_of_process;i++){
            System.out.print("P"+safe_sequence[i]+" ");
        }
        System.out.println();
    }
    

    public void submitRequest(int process_no, int[] request){
        for(int i=0;i<no_of_resources;i++){
            if(request[i] > need_resources[process_no][i]){
                System.out.println("Request cannot be granted as it exceeds the need of process P"+process_no);
                return;
            }
            if(request[i] > available_resources[i]){
                System.out.println("Request cannot be granted as it exceeds the available resources");
                return;
            }
        }

        // create copy of available resources, allocated resources and need resources
        int[] available_resources_copy = new int[no_of_resources];
        int[][] allocated_resources_copy = new int[no_of_process][no_of_resources];
        int[][] need_resources_copy =  new int[no_of_process][no_of_resources];

        // copy data
        for(int i=0;i<no_of_resources;i++){
            available_resources_copy[i] = available_resources[i];
        }
        for(int i=0;i<no_of_process;i++){
            for(int j=0;j<no_of_resources;j++){
                allocated_resources_copy[i][j] = allocated_resources[i][j];
                need_resources_copy[i][j] = need_resources[i][j];
            }
        }

        // grant request
        for(int i=0;i<no_of_resources;i++){
            available_resources_copy[i] -= request[i];
            allocated_resources_copy[process_no][i] += request[i];
            need_resources_copy[process_no][i] -= request[i];
        }        

        // create variables
        boolean[] finished = new boolean[no_of_process];
        int[] work = Arrays.copyOf(available_resources_copy, available_resources_copy.length);
        int[] safe_sequence_copy = new int[no_of_process];

        // calculate safe sequence
        int index = 0;
        boolean flag = false;


        while(true){
            for(int i=0;i<no_of_process;i++){
                if(finished[i] == false){
                    boolean canfinish = true;
                    for(int j=0;j<no_of_resources;j++){
                        if(need_resources_copy[i][j] > work[j]){
                            canfinish = false;
                            break;
                        }
                    }

                    if(canfinish){
                        for(int j=0;j<no_of_resources;j++){
                            work[j] += allocated_resources_copy[i][j];
                        }
                        finished[i] = true;
                        safe_sequence_copy[index++] = i;
                        flag = true;
                    }
                }
            }
            if(!flag) break;
            flag = false;
        }

        // check if safe sequence is valid
        for(int i=0;i<no_of_process;i++){
            if(finished[i] == false){
                System.out.println("Request cannot be granted as it will lead to unsafe state");
                return;
            }
        }

        System.out.println("Request can be granted as it will lead to safe state");
        System.out.print("Safe Sequence: ");
        for(int i=0;i<no_of_process;i++){
            System.out.print("P"+safe_sequence_copy[i]+" ");
        }
        System.out.println();

        // update data
        for(int i=0;i<no_of_resources;i++){
            available_resources[i] -= request[i];
            allocated_resources[process_no][i] += request[i];
            need_resources[process_no][i] -= request[i];
        }

        // check if need of process becomes zero
        boolean need_becomes_zero = true;
        for(int i=0;i<no_of_resources;i++){
            if(need_resources[process_no][i] != 0){
                need_becomes_zero = false;
                break;
            }
        }

        // if need of process becomes zero, add process to safe sequence
        if(need_becomes_zero){
            System.out.println("Need of process P"+process_no+" becomes zero, so it has finished execution");
            // free resources
            for(int i=0;i<no_of_resources;i++){
                available_resources[i] += allocated_resources[process_no][i];
                allocated_resources[process_no][i] = 0;
            }
        }
    }

}

public class Bankers {

  static int safe_sequence[];

  public static void main(String[] args) throws IOException {
    BufferedReader br = new BufferedReader(new FileReader("input.txt"));

    int nr = Integer.parseInt(br.readLine());
    int np = Integer.parseInt(br.readLine());

    BankersUtil banker = new BankersUtil(np, nr);

    // Read no of instances for each resource
    String[] resources = br.readLine().split(" ");
    for(int i=0;i<nr;i++){
        banker.setNoOfInstanceForResource(i, Integer.parseInt(resources[i]));
    }

    // Read max need for each process
    for(int i=0;i<np;i++){
        String[] _tmp = br.readLine().split(" ");
        for(int j=0;j<nr;j++){
            banker.setMaxResourceForProcess(i, j, Integer.parseInt(_tmp[j]));
        }
    }

    banker.calculateNeedMatrix();
    banker.displayData();
    while(true){
        System.out.println("Enter process number and request for each resource");
        BufferedReader br1 = new BufferedReader(new InputStreamReader(System.in));
        String[] _tmp = br1.readLine().split(" ");
        int process_no = Integer.parseInt(_tmp[0]);
        int[] request = new int[nr];
        for(int i=0;i<nr;i++){
            request[i] = Integer.parseInt(_tmp[i+1]);
        }
        banker.submitRequest(process_no, request);
        banker.displayData();
    } 
  }
}
