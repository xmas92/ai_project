package main;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.List;

import com.github.jreddit.entity.Comment;
import com.github.jreddit.entity.Submission;
import com.github.jreddit.entity.User;
import com.github.jreddit.retrieval.Comments;
import com.github.jreddit.retrieval.Submissions;
import com.github.jreddit.retrieval.params.CommentSort;
import com.github.jreddit.retrieval.params.SubmissionSort;
import com.github.jreddit.utils.restclient.HttpRestClient;
import com.github.jreddit.utils.restclient.Response;
import com.github.jreddit.utils.restclient.RestClient;

public class KorpusCreator {
	public static RestClient restClient;
	public static User user; 
	public static BufferedWriter bw; 
	
	public KorpusCreator() throws IOException {
		restClient = new HttpRestClient();
		restClient.setUserAgent("unique_user_agent"); //Your useragent please make it unique or BAN
		user = new User(restClient, "your_username", "your_password"); //Username and password
		File file = new File("worldnews.txt"); //<--------------------CHANGEHERE
		FileWriter fw = new FileWriter(file);
		bw = new BufferedWriter(fw);
	}
	
	public static void main(String[] args) throws IOException {
		new KorpusCreator();
		getSubmissions("worldnews"); // <--------------------------------CHANGE HERE
		Response r = restClient.get("http://reddit.com/user/swebonny", null);
		System.out.println(r.getResponseText());
		bw.close();
	}
	
	/**
	 * Retrieves the submissions and the comments from the specified
	 * subreddit. 
	 * @param subReddit The targetted subreddit. 
	 * @throws IOException
	 */
	public static void getSubmissions(String subReddit) throws IOException{
		Submissions subms = new Submissions(restClient, user);
		Comments comments = new Comments(restClient, user);
		List<Submission> submissionsSubreddit = subms.ofSubreddit(subReddit, SubmissionSort.TOP, -1, 60, null, null, true);

		for(Submission sub : submissionsSubreddit) {
			List<Comment> commentsFromArticle = comments.ofSubmission(sub, null, 8, 8, 10000, CommentSort.TOP);
			String s = sub.getSelftext() + " ";
			//System.out.println(sub.getSelftext());
			for(Comment c : commentsFromArticle) {
				s += c.getBody() + " ";
			}
			//System.out.println(s);
			bw.write(s);
			//writeString(s, sub.getAuthor());
		}
	}
	
	public static void writeString(String s, String title) throws IOException {
		File file = new File(title);
		FileWriter fw = new FileWriter(file);
		BufferedWriter bw = new BufferedWriter(fw);
		bw.write(s);
		bw.close();
		System.out.println("Finished writing");
	}
	
}
